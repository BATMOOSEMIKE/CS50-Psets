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

    userBalance = 0.0
    currentid = session["user_id"]

    portfolio = db.execute("SELECT symbol, shares FROM portfolio WHERE userid = :id", id = currentid)

    for stock in portfolio:
        symbol = stock["symbol"]
        shares = int(stock["shares"])
        stock = (lookup(symbol))
        price = float(stock["price"])
        total = shares*price
        userBalance = userBalance + total
        #update the price of the stock
        db.execute("UPDATE portfolio SET price = :price, value = :total WHERE userid = :id AND symbol = :symbol", price = price, total = total, id = currentid, symbol = symbol)

    pureCash = db.execute("SELECT cash FROM users WHERE id =:id", id = currentid)

    userBalance += pureCash[0]["cash"]

    portfolio = db.execute("SELECT * FROM portfolio WHERE userid = :id", id = currentid)

    return render_template("index.html", stocks = portfolio, cash = usd(pureCash[0]["cash"]), total = usd(userBalance))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":
        symbol = request.form.get("stockSymbol")

        sharesChecker = request.form.get("shareNumber")
        if not sharesChecker:
            return apology("You need to input a share number")

        shares = int(sharesChecker)
        symbolString = str(symbol)
        #Make sure symbol stays consistent
        symbolString = symbolString.upper()

        if not shares > 0:
            return apology("You cannot buy a negative amount of shares")
        stock = lookup(symbol)
        if not stock:
            return apology("The stock symbol you entered is invalid")

        currentid = session["user_id"]
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id = currentid)
        if float(cash[0]["cash"])<stock["price"]*shares:
            return apology("You don't have enough money to buy " + str(shares) + " of " + symbolString + " stock")

        existingShares = db.execute("SELECT shares from portfolio WHERE symbol = :symbol AND userid = :id", symbol = symbolString, id = currentid)

        if not existingShares:
            db.execute("INSERT INTO portfolio (userid, symbol, price, value, shares) VALUES(:id, :symbol, :price, :value, :shares)", id = currentid, symbol = symbolString, price = float(stock["price"]), value = float(stock["price"]*shares), shares = shares)
        else:
            db.execute("UPDATE portfolio SET shares = :shares + shares WHERE symbol = :symbol AND userid = :id", shares = shares, symbol = symbolString, id = currentid)

        db.execute("UPDATE users SET cash = cash - :price WHERE id = :id", price = float(shares) * float(stock["price"]), id = currentid)
        db.execute("INSERT INTO history(userid, symbol, price, shares, type, date) VALUES(:id, :symbol, :price, :shares, :type, CURRENT_TIMESTAMP)", id = currentid, symbol = symbolString, price = float(stock["price"]), shares = shares, type = "Buy")

        return redirect(url_for("index"))

    else:
        return render_template("buy.html")




@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    history = db.execute("SELECT * FROM history WHERE userid = :id", id = session["user_id"])
    return render_template("history.html", history = history)
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
    if request.method == "POST":
        StockSymbol = request.form.get("symbol")
        quote = lookup(StockSymbol)
        if not quote:
            return apology("The stock symbol you entered is invalid")
        else:
            return render_template("result.html", name = quote['name'], price = usd(quote['price']))

    return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        if not request.form.get("username"):
            return apology("Missing username!")
        if not request.form.get("password"):
            return apology("Missing password!")
        if not request.form.get("password") == request.form.get("passwordConfirm"):
            return apology("Passwords don't match")
        result = db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", username = request.form.get("username"), hash = pwd_context.hash(request.form.get("password")))
        if not result:
            return apology("Username is already taken")
        session["user_id"]=result
        return redirect(url_for("index"))
    else:
        return render_template("register.html")

@app.route("/change", methods=["GET", "POST"])
@login_required
def change():
    """Change password"""
    if request.method == "POST":
        if not request.form.get("password"):
            return apology("You must enter a password")

        db.execute("UPDATE users SET hash = :hash WHERE id = :id", hash = pwd_context.hash(request.form.get("password")), id = session["user_id"])
        return redirect(url_for("index"))


    return render_template("change.html")





@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    currentid = session["user_id"]
    if request.method == "POST":
        symbol = request.form.get("stockSymbol")

        sharesChecker = request.form.get("shareNumber")
        if not sharesChecker:
            return apology("You need to input a share number")

        sharesToSell = int(request.form.get("shareNumber"))
        symbolString = str(symbol)
        #Make sure symbol stays consistent
        symbolString.upper()

        if not sharesToSell > 0:
            return apology("You cannot sell a negative amount of shares")

        userStock = db.execute("SELECT * FROM portfolio WHERE symbol = :symbol AND userid = :id", symbol = symbolString, id = currentid)
        if not userStock:
            return apology("Sorry, you don't seem to own any shares of this stock")

        currentShares = db.execute("SELECT shares FROM portfolio WHERE symbol = :symbol AND userid =:id", symbol = symbolString, id = currentid)
        ownedShareNum = int(currentShares[0]["shares"])
        stockInfo = lookup(symbolString)
        stockPrice = float(stockInfo["price"])

        if (ownedShareNum < sharesToSell):
            return apology("Sorry, that's more shares than you own")

        balanceAddition = sharesToSell*stockPrice
        newShareNumber = ownedShareNum - sharesToSell


        db.execute("UPDATE users SET cash = cash + :add WHERE id = :id", id = currentid, add = balanceAddition)

        if(newShareNumber != 0):
            db.execute("UPDATE portfolio SET shares = :shares WHERE userid = :id AND symbol = :symbol", shares = newShareNumber, id = currentid, symbol = symbolString)
        else:
            db.execute("DELETE FROM portfolio WHERE userid = :id AND symbol = :symbol", id =currentid, symbol = symbolString)

        db.execute("INSERT INTO history(userid, symbol, price, shares, type, date) VALUES (:id, :symbol, :price, :shares, :type, CURRENT_TIMESTAMP)", id = currentid, symbol = symbolString,price = stockPrice, shares = sharesToSell, type = "Sell")

        return redirect(url_for("index"))

    else:
        return render_template("sell.html")
