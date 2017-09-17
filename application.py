from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    #create stocks list and query db for stocks owned
    stocks = []
    holdings = 0
    rows = db.execute("SELECT * FROM stock WHERE id = :user_id", user_id=session["user_id"])
    user = db.execute("SELECT * FROM users WHERE id = :user_id", user_id=session["user_id"])

    # interate over stocks to create list of dicts of stock info
    for x in rows[0].items():

        # get quote info for stock
        if x[0] == 'id':
            continue

        if x[1] == 0:
            continue

        quote = lookup('{}'.format(x[0]))


        holding = x[1] * quote["price"]
        stock = {
           "symbol": quote["symbol"],
           "name": quote["name"],
           "price": quote["price"],
           "units": x[1],
           "holding": round(holding, 2)
        }
        holdings += holding

        # append new stock to list of stocks
        stocks.append(stock)


    t_value = holdings + float(user[0]['cash'])
    return render_template("index.html", stocks=stocks, user=user[0]['username'], cash=round(user[0]['cash'], 2), holdings=round(holdings, 2), t_value=round(t_value, 2))
    #return apology("sorrrys")
@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
       # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        u = int(request.form.get("units"))

        # ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol")

        # ensure symbol exists
        elif not lookup(request.form.get("symbol")):
            return apology("symbol does not exist")

        # ensure units was submitted
        elif not request.form.get("units"):
            return apology("must provide number of units to buy")

        # ensure units is positive integer || HTML seems to do this by defualt when using type 'number'
        elif u < 1:
            return apology("units must be a positive integer")

        # get quote info
        quote = lookup(request.form.get("symbol"))

        # query database for user
        user = db.execute("SELECT * FROM users WHERE id = :user_id", user_id=session["user_id"])

        # calculate total price of buy
        t_price = round((quote["price"] * u), 2)

        # test if user has enough cash
        if t_price > float(user[0]["cash"]):
            return apology("not enough cash")
        else:
            # check if symbol is already in table, add if not
            sym_exist = db.execute("SELECT * FROM stock WHERE id = :user_id", user_id=session["user_id"])
            if not '{}'.format(quote["symbol"]) in sym_exist[0]:
                db.execute("ALTER TABLE stock ADD :sym INTEGER DEFAULT (0)", sym=quote["symbol"])
                sym_exist = db.execute("SELECT * FROM stock WHERE id = :user_id", user_id=session["user_id"])

            # calc new unit total and update
            new_units = sym_exist[0]['{}'.format(quote["symbol"])] + u
            db.execute("UPDATE stock SET :sym = :new_units WHERE id = :user_id", sym=quote["symbol"], new_units=new_units, user_id=session["user_id"])

            # subtract cost from user's cash
            new_cash = user[0]["cash"] - t_price
            db.execute("UPDATE users SET cash = :new_cash WHERE id = :user_id", new_cash=new_cash, user_id=session["user_id"])

            # add log of buy into history
            db.execute("INSERT INTO history (symb, units, price, price_total, transaction_type, id_user) VALUES (:symb, :units, :price, :price_total, 1, :id_user)", symb=quote["symbol"], units=u, price=quote["price"], price_total=t_price, id_user=session["user_id"])
            #db.execute("INSERT INTO history2 (symb) VALUES (:symb")
            # redirect user to home page
            return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")
@app.route("/history")
@login_required
def history():
    """Show history of transactions."""

    # get history
    try:
        rows = db.execute("SELECT * FROM history WHERE id_user = :user_id ORDER BY id_hist DESC", user_id=session["user_id"])
    except:
        return apology("NO TRANSACTIONS YET")

    # create empty history list to load into
    history = []

    num = len(rows) + 1
    for x in rows:
        # load each hist
        num -= 1
        hist = {
        "num": num,
        "symbol": x["symb"],
        #"name": x.quote["name"],
        "price": x["price"],
        "t_price": x["price_total"],
        "units": x["units"],
        "dt": x["date_time"],
        "id_hist": x["id_hist"]
        }

        if x["transaction_type"] == 0:
            hist['type'] = 'Sell'
        else:
            hist['type'] = 'Buy'
        # append each hist to the history list
        history.append(hist)


    return render_template("history.html", history=history)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # check if first login and make stock profile
        t_id = db.execute("SELECT id FROM stock WHERE id = :id", id=session["user_id"])

        if not t_id:
            stock = db.execute("INSERT INTO stock (id) VALUES (:id)", id=session["user_id"])


        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    else:
        s = request.form.get("symbol")
        quote = lookup(s)
        return render_template("quoted.html", symbol=quote['symbol'], price=quote['price'], name=quote['name'])

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # ensure password was submitted
        elif not request.form.get("Vpassword"):
            return apology("must verify password")

        # ensure passwords match
        t1 = request.form.get("password")
        t2 = request.form.get("Vpassword")
        if t1 != t2:
            return apology("passwords must match")

        # enter username into db
        hash = pwd_context.hash(t1)
        result = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)", username=request.form.get("username"), hash=hash)

        if not result:
            return apology("username already taken")

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
       # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        u = int(request.form.get("units"))

        # ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide symbol")

        # ensure symbol exists
        elif not lookup(request.form.get("symbol")):
            return apology("symbol does not exist")

        # ensure units was submitted
        elif not request.form.get("units"):
            return apology("must provide number of units to sell")

        # ensure units is positive integer || HTML seems to do this by defualt when using type 'number'
        elif u < 1:
            return apology("units must be a positive integer")

        # get profile stock info
        stocks = db.execute("SELECT * FROM stock WHERE id = :user_id", user_id=session["user_id"])

        # check if stock is in profile
        if not request.form.get("symbol").upper() in stocks[0]:
            return apology("None of that stock to sell")
        # check if enough stock exists in profile
        elif u > float(stocks[0][request.form.get("symbol").upper()]):
            return apology("Not enough stock")

        # activate sale
        else:
            # get quote info
            quote = lookup(request.form.get("symbol"))

            # query database for user
            user = db.execute("SELECT * FROM users WHERE id = :user_id", user_id=session["user_id"])

            # calculate total price of sale
            t_price = round((quote["price"] * u), 2)

            # calc new unit total and update
            new_units = stocks[0]['{}'.format(quote["symbol"])] - u
            db.execute("UPDATE stock SET :sym = :new_units WHERE id = :user_id", sym=quote["symbol"], new_units=new_units, user_id=session["user_id"])

            # add price to user's cash
            new_cash = user[0]["cash"] + t_price
            db.execute("UPDATE users SET cash = :new_cash WHERE id = :user_id", new_cash=new_cash, user_id=session["user_id"])

            # add log of buy into history
            db.execute("INSERT INTO history (symb, units, price, price_total, transaction_type, id_user) VALUES (:symb, :units, :price, :price_total, 0, :id_user)", symb=quote["symbol"], units=u, price=quote["price"], price_total=t_price, id_user=session["user_id"])

            # redirect user to home page
            return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("sell.html")

@app.route("/change", methods=["GET", "POST"])
@login_required
def change():
    """ Allows user to change password """

    if request.method == 'POST':

        # ensure username was submitted
        if not request.form.get("old_password"):
            return apology("must provide old password")

        # ensure password was submitted
        elif not request.form.get("new_password"):
            return apology("must provide new password")

        # ensure password was submitted
        elif not request.form.get("Vnew_password"):
            return apology("must verify new password")

        try:
            user = db.execute("SELECT * FROM users WHERE id = :user_id", user_id=session["user_id"])
        except:
            return apology('sorry!')

        p1 = request.form.get('new_password')
        p2 = request.form.get('Vnew_password')

        #verify old password
        if not pwd_context.verify(request.form.get("old_password"), user[0]["hash"]):
            return apology('old password incorrect')

        # verify new passwords match
        elif p1 != p2:
            return apology('passwords dont match')

        # change password
        else:
            new_hash = pwd_context.hash(request.form.get('new_password'))
            db.execute('UPDATE users SET hash = :new_hash WHERE id = :user_id', new_hash=new_hash, user_id=session['user_id'])

        return redirect(url_for("index"))

    else:
        return render_template('change.html')