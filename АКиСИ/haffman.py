import heapq, json
import os
import sys


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

    return heap[0]


def get_codes(node, prefix="", table=None):
    if table is None:
        table = {}

    if node.syml is not None:
        table[node.syml] = prefix
        return table

    get_codes(node.left, prefix + "0", table)
    get_codes(node.right, prefix + "1", table)

    return table


def encode(data, codes):
    return "".join(codes[c] for c in data)


def decode(encoded, tree):
    result = []
    node = tree

    for bit in encoded:
        node = node.left if bit == "0" else node.right
        if node.syml is not None:
            result.append(node.syml)
            node = tree

    return "".join(result)




with open(os.path.join(curr, "file.txt"), "r", encoding="utf-8") as f:
    text = f.read()




# ------------ Меню ------------
while True:
    print("\n1 — создать словарь Huffman (codes.json)")
    print("2 — закодировать файл (encoded.bin)")
    print("3 — декодировать файл (decode.txt)")
    print("4 — очистить папку")
    print("5 - Выйти из программы")
    choice = input("Выберите действие: ")


    if choice == "1":
        freqs = create_frequency_table(text)
        tree = build_huffman_tree(freqs)
        codes = get_codes(tree)

        with open(os.path.join(curr, "codes.json"), "w", encoding="utf-8") as f:
            json.dump(codes, f, ensure_ascii=False)

        print("Словарь Huffman создан: codes.json")


    elif choice == "2":
        if not os.path.exists(os.path.join(curr, "codes.json")):
            print("Ошибка: сначала создайте словарь (пункт 1).")
            continue

        with open(os.path.join(curr, "codes.json"), "r", encoding="utf-8") as f:
            codes = json.load(f)

        encoded = encode(text, codes)

        with open(os.path.join(curr, "encoded.bin"), "wb") as f:
            f.write(int(encoded, 2).to_bytes((len(encoded) + 7) // 8, "big"))

        print("Файл закодирован: encoded.bin")


    elif choice == "3":
        if not os.path.exists(os.path.join(curr, "codes.json")):
            print("Ошибка: нет codes.json!")
            continue

        if not os.path.exists(os.path.join(curr, "encoded.bin")):
            print("Ошибка: encoded.bin отсутствует!")
            continue

        with open(os.path.join(curr, "codes.json"), "r", encoding="utf-8") as f:
            codes = json.load(f)


        tree = Node(None, 0)
        for ch, code in codes.items():
            node = tree
            for bit in code:
                if bit == "0":
                    if not node.left:
                        node.left = Node(None, 0)
                    node = node.left
                else:
                    if not node.right:
                        node.right = Node(None, 0)
                    node = node.right
            node.syml = ch


        with open(os.path.join(curr, "encoded.bin"), "rb") as f:
            data = f.read()

        bit_string = bin(int.from_bytes(data, "big"))[2:]
        bit_string = bit_string.zfill(len(data) * 8)

        decoded = decode(bit_string, tree)

        with open(os.path.join(curr, "decode.txt"), "w", encoding="utf-8") as f:
            f.write(decoded)

        print("Файл декодирован: decode.txt")


    elif choice == "4":
        print("Удаляю временные файлы...")

        self_file = os.path.basename(sys.argv[0])

        for file in os.listdir(curr):
            if file in (self_file, "file.txt"):
                continue

            path = os.path.join(curr, file)

            if os.path.isfile(path):
                os.remove(path)
                print("Удалено:", file)
    elif choice == "5":    
        print("Готово. Выход.")
        break

    else:
        print("Нет такого действия.")
