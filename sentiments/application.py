from flask import Flask, redirect, render_template, request, url_for

import helpers
import nltk
import os
import sys
from nltk.tokenize import TweetTokenizer
from analyzer import Analyzer

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():
    # absolute paths to lists
    positives = os.path.join(sys.path[0], "positive-words.txt")
    negatives = os.path.join(sys.path[0], "negative-words.txt")

    # instantiate analyzer
    analyzer = Analyzer(positives, negatives)

    # validate screen_name
    screen_name = request.args.get("screen_name", "")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets
    tweets = helpers.get_user_timeline(screen_name, count=100)

    # tonkenize and analyze
    tknzr = TweetTokenizer(strip_handles=True, reduce_len=True)

    positive, negative, neutral = 0.0, 0.0, 0.0

    for i in range(len(tweets)):
        tweet_words = tknzr.tokenize(tweets[i])
        score = 0
        for j in range(len(tweet_words)):
            score += analyzer.analyze(tweet_words[j])
        if score > 0.0:
            positive += 1
        elif score < 0.0:
            negative += 1
        else:
            neutral += 1

    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
