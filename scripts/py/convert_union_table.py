import logging
import time

logging.basicConfig(filename='convert.log', level=logging.DEBUG,
                    format='%(asctime)s %(levelname)s: %(message)s')

def find_w12_from_w(word):
    w1, w2 = word.split("∩")
    return w1, w2

def get_MM_from_inverted_file(num, path):
    n = num
    word_num = 0
    MM = {}
    with open(path, 'r') as f:
        for line in f:
            if n <= 0:
                break
            line = line.strip()
            words = line.split()
            w = words[0]
            ids = words[1:]
            MM[w] = ids
            n -= len(ids)
            word_num += 1
    logging.info("convert num: {}".format(num - n))
    print("convert num: {}".format(num - n))
    logging.info("words num: {}".format(word_num))
    print("words num: {}".format(word_num))
    return MM

def convert_union_file(inverted_MM, out_fpath):
    sum = 0
    with open(out_fpath, 'a') as f:
        for i, (w1, ids1) in enumerate(inverted_MM.items()):
            set_ids1 = set(ids1)
            for j, (w2, ids2) in enumerate(list(inverted_MM.items())[i+1:], start=i+1):
                set_ids2 = set(ids2)
                inters = set_ids1.intersection(set_ids2)
                if len(inters) != 0:
                    inters_list = list(inters)
                    f.write(f"{w1}∩{w2} ")
                    for z in inters_list:
                        f.write(f"{z} ")
                    f.write("\n")
                    sum += 1
    logging.info("sum: {}".format(sum))
    print("sum:", sum)

if __name__ == "__main__":
    mm_num = int(input("please input word num："))
    # mm_num = 10000000
    in_fpath = '../../testData/inverted_18.txt'
    out_fpath = '../../testData/test_py_10_7.txt'
    start_time = time.time()
    inverted_MM = get_MM_from_inverted_file(mm_num, in_fpath)
    convert_union_file(inverted_MM, out_fpath)
    logging.info("time cost: {} s".format(time.time() - start_time))
    print("time cost: ",  time.time() - start_time, "s")
    logging.info("convert success! (num: {})".format(mm_num))
    print("convert success! (num:", mm_num, ")")