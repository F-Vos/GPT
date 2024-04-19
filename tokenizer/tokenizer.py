import re



def train():
    return None


def encode():
    return None


def decode():
    return None

text2 = "This is a test text for the tokenizer. It should tokenize this text properly."
text = "aaabdaaabac"


tokens = text.encode("utf-8")
tokens = list(map(int, tokens))
print(text)
print("---")
print(tokens)

def get_pairs(ids):
    counts = {}
    for pair in zip(ids, ids[1:]):
        counts[pair] = counts.get(pair, 0) + 1
    return counts

stats =  get_pairs(tokens)
print(stats)
