import nltk

class Analyzer():
    """Implements sentiment analysis."""



    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        self.pos_words = []
        self.neg_words = []

        f = open(negatives, 'r')

        for line in f:
            if line[0] != ';':
                line = line.strip('\n')
                self.neg_words.append(line)

        f.close()

        self.neg_words.remove('')

        f = open(positives, 'r')

        for line in f:
            if line[0] != ';':
                line = line.strip('\n')
                self.pos_words.append(line)

        f.close()

        self.pos_words.remove('')

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        score = 0

        text = str.lower(text)

        if text in self.pos_words:
            score += 1
        if text in self.neg_words:
            score -= 1

        return score;


