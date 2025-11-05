import heapq, json
import os
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
    for c in data: freqs[c] = freqs.get(c, 0) + 1
    return freqs

def build_huffman_tree(freqs):
    heap = [Node(ch,f) for ch,f in freqs.items()]
    heapq.heapify(heap)
    while len(heap) > 1:
        l, r = heapq.heappop(heap), heapq.heappop(heap)
        heapq.heappush(heap, Node(None, l.freq+r.freq, l, r))
    return heap[0]

def get_codes(node, prefix="", table=None):
    if table is None: table = {}
    if node.syml is not None:
        table[node.syml] = prefix
        return table
    get_codes(node.left, prefix+"0", table)
    get_codes(node.right, prefix+"1", table)
    return table

def encode(data, codes):
    return "".join(codes[c] for c in data)

def decode(encoded, tree):
    result, node = [], tree
    for bit in encoded:
        node = node.left if bit == "0" else node.right
        if node.syml is not None:
            result.append(node.syml)
            node = tree
    return "".join(result)
# ---------------------------
# ENCODING FILE
# ---------------------------
with open(r'АКиСИ/file.txt', "r", encoding="utf-8") as f:
    text = f.read()


freqs = create_frequency_table(text)
tree = build_huffman_tree(freqs)
codes = get_codes(tree)
encoded = encode(text, codes)
decoded = decode(encoded, tree)

with open(os.path.join(curr,"encoded.txt"), "wb") as f:
    f.write(int(encoded, 2).to_bytes((len(encoded)+7)//8, byteorder="big"))

with open(os.path.join(curr,"codes.json"),"w", encoding="utf-8") as f:
    json.dump(codes, f)
print(" Файл закодирован: encoded.txt")
print(" Таблица кодов сохранена: codes.json")

with open(os.path.join(curr,"decode.txt"), "w", encoding="utf-8") as f:
    f.write(decoded)


if text == decoded:
    print("Декодирование успешно: исходный текст совпадает с декодированным.")
else:
    print("Ошибка: декодированный текст не совпадает с исходным.")

