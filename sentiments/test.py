

pos_words = []
neg_words = []

f = open('negative-words.txt', 'r')

for line in f:
    if line[0] != ';':
        line = line.strip('\n')
        neg_words.append(line)

f.close()

neg_words.remove('')

f = open('positive-words.txt', 'r')

for line in f:
    if line[0] != ';':
        line = line.strip('\n')
        pos_words.append(line)

f.close()

pos_words.remove('')