import os
import pprint
import requests
import json

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    rows1 = db.execute("SELECT symbol, name, shares, total FROM bought WHERE id = :user_id", user_id=session["user_id"])
    rows2 = db.execute("SELECT cash FROM users WHERE id = :user_id",
                      user_id=session["user_id"])
    total_f = rows2[0]["cash"]
    price_symbol = []
    for i in range(0, len(rows1)):
        total_f += rows1[i]["total"]
        rows1[i]["total"] = "${:,.2f}".format(rows1[i]["total"])
        r = requests.get('https://cloud-sse.iexapis.com/stable/stock/{}/quote?token={}'.format(rows1[i]["symbol"], os.environ.get("API_KEY")))
        files = r.json()
        price_symbol.append(files['latestPrice'])
        price_symbol[i] = "${:,.2f}".format(price_symbol[i])
    cash = "${:,.2f}".format(rows2[0]["cash"])
    total_f = "${:,.2f}".format(total_f)
    return render_template("index.html", cash=cash, rows1=rows1, len=len(rows1), latestPrice=price_symbol, total_f=total_f)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)
        symbol = request.form.get("symbol")
        r = requests.get('https://cloud-sse.iexapis.com/stable/stock/{}/quote?token={}'.format(symbol, os.environ.get("API_KEY")))
        if not r:
            return apology("invalid symbol", 400)
        files = r.json()
        id = session["user_id"]
        symbol = files['symbol']
        name = files['companyName']
        price = round(float(files['latestPrice']), 4)
        shares = int(request.form.get("shares"))
        total = price * shares
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=id)
        if total > cash[0]['cash']:
            return apology("can't afford", 400)
        else:
            db.execute("INSERT INTO history (id, symbol, name, shares, price, total, transacted) VALUES (:id, :symbol, :name, :shares, :price, :total, CURRENT_TIMESTAMP)",
                       id=id, symbol=symbol, name=name, price=price, shares=shares, total=total)
            r_bought = db.execute("SELECT * FROM bought WHERE id = :id AND symbol = :symbol", id=id, symbol=symbol)
            if not r_bought:
                db.execute("INSERT INTO bought (id, symbol, name, shares, total) VALUES (:id, :symbol, :name, :shares, :total)",
                           id=id, symbol=symbol, name=name, shares=shares, total=total)
            else:
                db.execute("UPDATE bought SET shares = shares + " + str(shares) + ", total = total + " + str(total) +" WHERE id = :id AND symbol = :symbol",
                           id=id, symbol=symbol)
        db.execute("UPDATE users SET cash = cash - " + str(total) + " WHERE id = :id", id=id)
        flash('Bought!')
        return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    rows = db.execute("SELECT symbol, shares, price, transacted FROM history WHERE id = :id", id=session["user_id"])
    print(rows)
    for i in range(0, len(rows)):
        rows[i]["price"] = "${:,.2f}".format(rows[i]["price"])
    return render_template("history.html", rows=rows, len=len(rows))


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    else:
        symbol = request.form.get("symbol")
        r = requests.get('https://cloud-sse.iexapis.com/stable/stock/{}/quote?token={}'.format(symbol, os.environ.get("API_KEY")))
        if not r:
            return apology("invalid symbol", 400)
        files = r.json()
        return render_template("quoted.html", symbol=symbol, latestPrice=files['latestPrice'], companyName=files['companyName'])


@app.route("/check", methods=["POST"])
def check():
    rows = db.execute("SELECT * FROM users WHERE username = :username",
                 username=request.form.get("username"))

    if not request.form.get("username"):
        return "false"
    elif len(rows) == 1:
        return "false"
    else:
        return "success"


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    else:
        if not request.form.get("password"):
            return apology("missing password", 400)
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("password don't match", 400)
        else:
            username = request.form.get("username")
            password = request.form.get("password")
            hash_p = generate_password_hash(password, method='pbkdf2:sha256', salt_length=len(password))
            db.execute("INSERT INTO users (username, hash, cash) VALUES (:username, :hash, :cash)",
                       username=username, hash=hash_p, cash=10000)

            rows = db.execute("SELECT * FROM users WHERE username = :username",
                   username=request.form.get("username"))
            session["user_id"] = rows[0]["id"]
            flash('Registered!')
            return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        rows = db.execute("SELECT symbol FROM bought WHERE id = :user_id", user_id=session["user_id"])
        return render_template("sell.html", rows=rows, length=len(rows))
    else:
        symbol = request.form.get("symbol")
        shares = -int(request.form.get("shares"))
        r = requests.get('https://cloud-sse.iexapis.com/stable/stock/{}/quote?token={}'.format(symbol, os.environ.get("API_KEY")))
        files = r.json()
        l_price = files['latestPrice']
        name = files['companyName']
        tt_sold = shares * round(float(l_price), 4)
        shares_o = db.execute("SELECT shares FROM bought WHERE id= :id AND symbol = :symbol", id=session["user_id"], symbol=symbol)
        if abs(shares) > shares_o[0]["shares"]:
            return apology("too many shares", 400)
        else:
            if abs(shares) == shares_o[0]["shares"]:
                db.execute("DELETE FROM bought WHERE id= :id AND symbol = :symbol", id=session["user_id"], symbol=symbol)

                db.execute("INSERT INTO history (id, symbol, name, shares, price, total, transacted) VALUES (:id, :symbol, :name, :shares, :price, :total, CURRENT_TIMESTAMP)",
                           id=session["user_id"], symbol=symbol, name=name, price=l_price, shares=shares, total=tt_sold)

                db.execute("UPDATE users SET cash = cash - " + str(tt_sold) + " WHERE id = :id", id=session["user_id"])
                flash('Sold!')
                return redirect("/")
            else:
                db.execute("INSERT INTO history (id, symbol, name, shares, price, total, transacted) VALUES (:id, :symbol, :name, :shares, :price, :total, CURRENT_TIMESTAMP)",
                           id=session["user_id"], symbol=symbol, name=name, price=l_price, shares=shares, total=tt_sold)

                db.execute("UPDATE bought SET shares = shares + " + str(shares) + ", total = total + " + str(tt_sold) + " WHERE id = :id AND symbol = :symbol",
                           id=session["user_id"], symbol=symbol)

                db.execute("UPDATE users SET cash = cash - " + str(tt_sold) + " WHERE id = :id", id=session["user_id"])
                flash('Sold!')
                return redirect("/")



def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
