import heapq
import os
import sys
import struct

curr = os.path.dirname(os.path.abspath(__file__))


class Node:
    def __init__(self, syml, freq, left=None, right=None):
        self.syml = syml
        self.freq = freq
        self.left = left
        self.right = right

    def __lt__(self, other):
        return self.freq < other.freq


def create_frequency_table(data):
    freqs = {}
    for c in data:
        freqs[c] = freqs.get(c, 0) + 1
    return freqs


def build_huffman_tree(freqs):
    heap = [Node(ch, f) for ch, f in freqs.items()]
    heapq.heapify(heap)

    while len(heap) > 1:
        l = heapq.heappop(heap)
        r = heapq.heappop(heap)
        heapq.heappush(heap, Node(None, l.freq + r.freq, l, r))

    return heap[0] if heap else None


def get_codes(node, prefix="", table=None):
    if table is None:
        table = {}

    if node is None:
        return table

    if node.syml is not None:
        table[node.syml] = prefix
        return table

    get_codes(node.left, prefix + "0", table)
    get_codes(node.right, prefix + "1", table)

    return table


def encode(data, codes):
    return "".join(codes[c] for c in data)


def decode(encoded, tree):
    if not tree:
        return ""
    result = []
    node = tree
    for bit in encoded:
        node = node.left if bit == "0" else node.right
        if node.syml is not None:
            result.append(node.syml)
            node = tree
    return "".join(result)


def build_canonical_codes(lengths):
   
    items = [(lengths[s], s) for s in lengths]
    items.sort(key=lambda x: (x[0], x[1].encode('utf-8')))
    codes = {}
    if not items:
        return codes
    prev_len = items[0][0]
    code = 0
    for length, sym in items:
        if length != prev_len:
            code <<= (length - prev_len)
            prev_len = length
        bits = format(code, 'b').rjust(length, '0')
        codes[sym] = bits
        code += 1
    return codes


def make_decoding_tree_from_codes(codes):
    root = Node(None, 0)
    for sym, bits in codes.items():
        node = root
        for b in bits:
            if b == '0':
                if not node.left:
                    node.left = Node(None, 0)
                node = node.left
            else:
                if not node.right:
                    node.right = Node(None, 0)
                node = node.right
        node.syml = sym
    return root


def write_archive(path, symbols_and_lengths, bit_length, payload_bytes):
    with open(path, 'wb') as f:
        f.write(b'HUF2')
        f.write(struct.pack('<H', len(symbols_and_lengths)))
        for sym, length in symbols_and_lengths:
            b = sym.encode('utf-8')
            if len(b) > 65535:
                raise ValueError('Symbol too long')
            f.write(struct.pack('<H', len(b)))
            f.write(b)
            f.write(struct.pack('<B', length))
        f.write(struct.pack('<I', bit_length))
        f.write(payload_bytes)


def read_archive(path):
    with open(path, 'rb') as f:
        magic = f.read(4)
        if magic != b'HUF2':
            raise ValueError('Not a valid HUF2 archive')
        (count,) = struct.unpack('<H', f.read(2))
        symbols_and_lengths = []
        for _ in range(count):
            (llen,) = struct.unpack('<H', f.read(2))
            s = f.read(llen).decode('utf-8')
            (length,) = struct.unpack('<B', f.read(1))
            symbols_and_lengths.append((s, length))
        (bit_length,) = struct.unpack('<I', f.read(4))
        payload = f.read()
    return symbols_and_lengths, bit_length, payload


# ---------------------------------------
#              MAIN
# ---------------------------------------

file_txt = os.path.join(curr, 'file.txt')
archive_file = os.path.join(curr, 'archive.huf')
decoded_file = os.path.join(curr, 'decode.txt')

if not os.path.exists(file_txt):
    open(file_txt, 'w', encoding='utf-8').close()

with open(file_txt, 'r', encoding='utf-8') as f:
    text = f.read()

MENU = '''
1 — закодировать файл (создаёт archive.huf)
2 — декодировать файл (читает archive.huf и пишет decode.txt)
3 — словарь (codes.txt)
4 — очистить папку 
5 — выйти
'''

while True:
    print(MENU)
    choice = input('Выберите действие: ')

    if choice == '1':
        freqs = create_frequency_table(text)
        tree = build_huffman_tree(freqs)
        codes = get_codes(tree)
        lengths = {s: len(b) for s, b in codes.items()}
        items = [(lengths[s], s) for s in lengths]
        items.sort(key=lambda x: (x[0], x[1].encode('utf-8')))
        canonical_order = [(s, l) for l, s in items]
        canonical_codes = build_canonical_codes(lengths)

        encoded_bits = encode(text, canonical_codes)
        bit_length = len(encoded_bits)
        payload = int(encoded_bits, 2).to_bytes((bit_length + 7) // 8, 'big') if bit_length > 0 else b''

        write_archive(archive_file, canonical_order, bit_length, payload)
        print(f'Файл закодирован и сохранён в {archive_file} (символов: {len(canonical_order)}, битов: {bit_length})')

    elif choice == '2':
        if not os.path.exists(archive_file):
            print('Ошибка: archive.huf отсутствует! Сначала кодируйте (пункт 1).')
            continue
        symbols_and_lengths, bit_length, payload = read_archive(archive_file)
        lengths = {s: l for s, l in symbols_and_lengths}
        canonical_codes = build_canonical_codes(lengths)
        tree = make_decoding_tree_from_codes(canonical_codes)

        bit_string = ''.join(format(byte, '08b') for byte in payload)
        if isinstance(bit_length, int):
            bit_string = bit_string[:bit_length]

        decoded = decode(bit_string, tree)
        with open(decoded_file, 'w', encoding='utf-8') as f:
            f.write(decoded)
        print(f'Файл декодирован и сохранён в {decoded_file}')

    elif choice == '3':
   
        if not os.path.exists(archive_file):
            print('archive.huf не найден — сначала создайте архив (пункт 1).')
            continue
        symbols_and_lengths, bit_length, payload = read_archive(archive_file)
        lengths = {s: l for s, l in symbols_and_lengths}
        canonical_codes = build_canonical_codes(lengths)

        codes_path = os.path.join(curr, 'codes.txt')
        with open(codes_path, 'w', encoding='utf-8') as cf:
            for sym, length in symbols_and_lengths:
                codebits = canonical_codes[sym]
                cf.write(f"{repr(sym)}: {codebits}\n")
        print(f'Словарь сохранён в {codes_path}')

    elif choice == '4':
        print('Удаляю временные файлы...')
        self_file = os.path.basename(sys.argv[0])
        for file in os.listdir(curr):
            if file in (self_file, 'file.txt'):
                continue
            path = os.path.join(curr, file)
            if os.path.isfile(path):
                os.remove(path)
                print('Удалено:', file)

    elif choice == '5':
        print('Готово. Выход.')
        break

    else:
        print('Нет такого действия.')
