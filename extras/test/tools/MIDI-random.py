import random
import os
import os.path as osp

outdir = osp.join(osp.dirname(__file__), "data")
os.makedirs(outdir, exist_ok=True)


MAX_SYSEX_LEN = 2048


def random_channelmsg():
    h = random.randint(0x80, 0xEF)
    d1 = random.randint(0x00, 0x7F)
    d2 = random.randint(0x00, 0x7F)
    return [h, d1] if 0xC0 <= h <= 0xDF else [h, d1, d2]


def random_sysexmsg(l=None):
    if l is None:
        l = random.randint(1, MAX_SYSEX_LEN)
    return [0xF0] + [random.randint(0x00, 0x7F) for _ in range(l)] + [0xF7]


def random_realtimemsg():
    return [random.choice([0xF8, 0xFA, 0xFB, 0xFC, 0xFF])]


def random_syscommonmsg():
    h = random.choice([0b1111_0001, 0b1111_0010, 0b1111_0011, 0b1111_0111])
    d1 = random.randint(0x00, 0x7F)
    d2 = random.randint(0x00, 0x7F)
    return {
        0b1111_0001: [h, d1],
        0b1111_0010: [h, d1, d2],
        0b1111_0011: [h, d1],
        0b1111_0111: [h],
    }[h]


def random_msg():
    msgs = [
        random_channelmsg,
        random_sysexmsg,
        random_realtimemsg,
        random_syscommonmsg,
    ]
    return random.choice(msgs)()


random.seed(0)

MAX_SYSEX_LEN = 2048
challenge = [random_msg() for _ in range(10000)]

with open(osp.join(outdir, "MIDI-long-challenge.syx"), "wb") as f:
    for chg in challenge:
        f.write(bytes(chg))

MAX_SYSEX_LEN = 256
challenge = [random_msg() for _ in range(10000)]

with open(osp.join(outdir, "MIDI-short-challenge.syx"), "wb") as f:
    for chg in challenge:
        f.write(bytes(chg))
