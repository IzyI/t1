from pprint import pprint as pp


def print_rozetki_define():
    count = 1
    for i in range(0, 64 * 8, 64):
        print(f"#define ROZETKA_{count}   0x{convert_base(str(i), from_base=10, to_base=16)}")
        count+=1


def convert_base(num, to_base=10, from_base=10):
    # сначала переводим десятичные
    if isinstance(num, str):
        n = int(num, from_base)
    else:
        n = int(num)
    # переводим десятичные в произвольную
    alphabet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    if n < to_base:
        return alphabet[n]
    else:
        return convert_base(n // to_base, to_base) + alphabet[n % to_base]


# 0xA0 = A0
# result =
# pp(f"{result}")
#
print_rozetki_define()
min_start=15
min_check=10
min_stop=5
print((min_start<min_check) & (min_check>=min_stop))

min_start=10
min_check=5
min_stop=0
print((min_start<min_check) | (min_check>=min_stop))