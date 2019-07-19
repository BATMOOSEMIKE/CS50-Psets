import nltk

class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        
        self.positives = list()
        self.negatives = list()

        file = open(positives, "r")
        for line in file:
            if line.startswith(";") == False:
                self.positives.append(line.strip())
        file.close()

        file = open(negatives, "r")
        for line in file:
            if line.startswith(";") == False:
                self.negatives.append(line.strip())
        file.close()
    

        


    def analyze(self, text):
        
        score = 0
        

        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)

        for word in tokens:
            if word.lower() in self.positives:
                score += 1
            elif word.lower() in self.negatives:
                score += -1
                
        
        return score
