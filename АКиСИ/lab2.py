import struct
import time
import os
from collections import Counter

# =======================
# АРИФМЕТИЧЕСКИЙ КОДЕР
# =======================

class ArithmeticCoder:
    def __init__(self):
        self.MAX_RANGE = 1 << 32

    def build_cumulative(self, freq):
        symbols = sorted(freq.keys())
        cum = [0]
        for s in symbols:
            cum.append(cum[-1] + freq[s])
        return symbols, cum

    # ---------- ENCODE ----------
    def encode_file(self, input_file, output_file):
        # Попробовать открыть файл как указано; если не найден — попытаться в каталоге скрипта
        if not os.path.exists(input_file):
            script_dir = os.path.dirname(__file__)
            alt_path = os.path.join(script_dir, input_file)
            if os.path.exists(alt_path):
                input_file = alt_path
            else:
                print(f"Ошибка: Файл {input_file} не найден!")
                return False

        try:
            with open(input_file, 'r', encoding='utf-8') as f:
                text = f.read()
        except Exception as e:
            print(f"Ошибка при чтении {input_file}: {e}")
            return False

        if not text:
            print("Файл пуст")
            return False

        freq = Counter(text)
        symbols, cum = self.build_cumulative(freq)
        total = cum[-1]

        
        class BitWriter:
            def __init__(self):
                self.buf = 0
                self.n = 0
                self.bytes = bytearray()

            def put_bit(self, b):
                self.buf = (self.buf << 1) | (1 if b else 0)
                self.n += 1
                if self.n == 8:
                    self.bytes.append(self.buf)
                    self.buf = 0
                    self.n = 0

            def put_bits(self, b, count):
                for i in range(count - 1, -1, -1):
                    self.put_bit((b >> i) & 1)

            def flush(self):
                if self.n > 0:
                    self.buf <<= (8 - self.n)
                    self.bytes.append(self.buf)

    
        MASK = (1 << 32) - 1
        HALF = 1 << 31
        QUARTER = 1 << 30

        low = 0
        high = MASK
        underflow = 0
        writer = BitWriter()

        
        sym_to_idx = {s: i for i, s in enumerate(symbols)}

        for ch in text:
            i = sym_to_idx[ch]
            range_ = high - low + 1
            high = low + (range_ * cum[i + 1]) // total - 1
            low = low + (range_ * cum[i]) // total

            # renormalize
            while True:
                if high < HALF:
                    writer.put_bit(0)
                    for _ in range(underflow):
                        writer.put_bit(1)
                    underflow = 0
                    low = (low << 1) & MASK
                    high = ((high << 1) & MASK) | 1
                elif low >= HALF:
                    writer.put_bit(1)
                    for _ in range(underflow):
                        writer.put_bit(0)
                    underflow = 0
                    low = ((low - HALF) << 1) & MASK
                    high = (((high - HALF) << 1) & MASK) | 1
                elif low >= QUARTER and high < 3 * QUARTER:
                    underflow += 1
                    low = ((low - QUARTER) << 1) & MASK
                    high = (((high - QUARTER) << 1) & MASK) | 1
                else:
                    break

        
        underflow += 1
        if low < QUARTER:
            writer.put_bit(0)
            for _ in range(underflow):
                writer.put_bit(1)
        else:
            writer.put_bit(1)
            for _ in range(underflow):
                writer.put_bit(0)

        writer.flush()
        bit_length = len(writer.bytes) * 8

        with open(output_file, 'wb') as f:
            # длина текста (число символов)
            f.write(struct.pack('I', len(text)))
            # количество символов (в таблице частот)
            f.write(struct.pack('H', len(freq)))
            # таблица частот: для каждого символ - длина байт (H), байты, частота (I)
            for ch, fr in freq.items():
                b = ch.encode('utf-8')
                f.write(struct.pack('H', len(b)))
                f.write(b)
                f.write(struct.pack('I', fr))
            # битовая длина и payload
            f.write(struct.pack('I', bit_length))
            f.write(bytes(writer.bytes))

        return True

    # ---------- DECODE ----------
    def decode_file(self, input_file, output_file):
        # Попробовать путь как есть, иначе поискать рядом со скриптом
        if not os.path.exists(input_file):
            script_dir = os.path.dirname(__file__)
            alt_path = os.path.join(script_dir, input_file)
            if os.path.exists(alt_path):
                input_file = alt_path
            else:
                print(f"Ошибка: Файл {input_file} не найден!")
                return False

        try:
            with open(input_file, 'rb') as f:
                text_len = struct.unpack('I', f.read(4))[0]
                sym_count = struct.unpack('H', f.read(2))[0]

                freq = {}
                for _ in range(sym_count):
                    blen = struct.unpack('H', f.read(2))[0]
                    b = f.read(blen)
                    fr = struct.unpack('I', f.read(4))[0]
                    freq[b.decode('utf-8')] = fr

                bit_length = struct.unpack('I', f.read(4))[0]
                payload = f.read()
        except Exception as e:
            print(f"Ошибка при чтении {input_file}: {e}")
            return False

        symbols, cum = self.build_cumulative(freq)
        total = cum[-1]

        # Bit reader
        class BitReader:
            def __init__(self, data, bitlen):
                self.data = data
                self.bitlen = bitlen
                self.pos = 0

            def next_bit(self):
                if self.pos >= self.bitlen:
                    return 0
                byte_idx = self.pos // 8
                bit_idx = 7 - (self.pos % 8)
                b = (self.data[byte_idx] >> bit_idx) & 1
                self.pos += 1
                return b

        br = BitReader(payload, bit_length)

        
        MASK = (1 << 32) - 1
        HALF = 1 << 31
        QUARTER = 1 << 30

        
        value = 0
        for _ in range(32):
            value = (value << 1) | br.next_bit()

        low = 0
        high = MASK

        result = []

        for _ in range(text_len):
            range_ = high - low + 1
            scaled = ((value - low + 1) * total - 1) // range_

            # find symbol index
            for i in range(len(symbols)):
                if cum[i] <= scaled < cum[i + 1]:
                    ch = symbols[i]
                    break

            result.append(ch)

            high = low + (range_ * cum[i + 1]) // total - 1
            low = low + (range_ * cum[i]) // total

            # renormalize
            while True:
                if high < HALF:
                    pass
                elif low >= HALF:
                    low -= HALF
                    high -= HALF
                    value -= HALF
                elif low >= QUARTER and high < 3 * QUARTER:
                    low -= QUARTER
                    high -= QUARTER
                    value -= QUARTER
                else:
                    break

                low = (low << 1) & MASK
                high = ((high << 1) & MASK) | 1
                value = ((value << 1) & MASK) | br.next_bit()

        with open(output_file, 'w', encoding='utf-8') as f:
            f.write(''.join(result))

        return True


# ------------------
# Утилита: показать/сохранить таблицу частот
# ------------------
def write_freq_table(input_file="file.txt", out_file="file_freq.txt"):
    # Попытка найти входной файл рядом со скриптом, если не найден в CWD
    if not os.path.exists(input_file):
        script_dir = os.path.dirname(__file__)
        alt_path = os.path.join(script_dir, input_file)
        if os.path.exists(alt_path):
            input_file = alt_path
        else:
            print(f"Ошибка: Файл {input_file} не найден!")
            return False

    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            text = f.read()
    except Exception as e:
        print(f"Ошибка при чтении {input_file}: {e}")
        return False

    freq = Counter(text)
    items = sorted(freq.items(), key=lambda x: -x[1])

    print("Топ символов (символ -> частота):")
    for ch, fr in items[:40]:
        print(f"{repr(ch)}: {fr}")

    # Сохраняем таблицу рядом с входным файлом (если out_file относительный)
    out_dir = os.path.dirname(input_file) or os.getcwd()
    out_path = out_file if os.path.isabs(out_file) else os.path.join(out_dir, out_file)
    try:
        with open(out_path, 'w', encoding='utf-8') as f:
            f.write(f"Total chars: {len(text)}\n")
            for ch, fr in items:
                f.write(f"{repr(ch)}\t{fr}\n")
        print(f"Таблица частот сохранена в {out_path}")
    except Exception as e:
        print(f"Ошибка при записи {out_path}: {e}")
        return False

    return True


# =======================
# МЕНЮ
# =======================

def main():
    coder = ArithmeticCoder()

    # Все рабочие файлы будут храниться в папке скрипта (чтобы всё было в одной папке)
    script_dir = os.path.dirname(__file__)
    input_file = os.path.join(script_dir, "file.txt")
    archive_file = os.path.join(script_dir, "file.arith")
    decoded_file = os.path.join(script_dir, "file_decoded.txt")
    freq_file = os.path.join(script_dir, "file_freq.txt")

    while True:
        print(f"\n1. Кодировать ")
        print(f"2. Декодировать ")
        print(f"3. Показать/сохранить таблицу частот ")
        print("0. Выход")

        choice = input("Выбор: ").strip()

        if choice == '0':
            break

        if choice == '1':
            start = time.time()
            ok = coder.encode_file(input_file, archive_file)
            if ok:
                print("Кодирование завершено")
                print("Время:", round(time.time() - start, 3), "сек")

        elif choice == '2':
            start = time.time()
            ok = coder.decode_file(archive_file, decoded_file)
            if ok:
                print("Декодирование завершено")
                print("Время:", round(time.time() - start, 3), "сек")

                if os.path.exists(input_file):
                    with open(input_file, encoding='utf-8') as a, \
                         open(decoded_file, encoding='utf-8') as b:
                        print("LOSSLESS " if a.read() == b.read() else "ОШИБКА ")

        elif choice == '3':
            # Показать и сохранить таблицу частот
            write_freq_table(input_file, freq_file)

        else:
            print("Неверный выбор. Пожалуйста, введите 0-3.")

if __name__ == "__main__":
    main()
