def main():
    c = [i for i in range(10)]
    for i in range(1024):
        L = []
        for j in range(10):
            if i & (1 << j):
                L.append(j)

if __name__ == '__main__':
    main()
