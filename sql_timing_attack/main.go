package main

import (
    "database/sql"
    "fmt"
    "log"
    "net/http"
    _ "github.com/mattn/go-sqlite3"
    _"time"
)

var db *sql.DB

func main() {
    var err error
    db, err = sql.Open("sqlite3", "./test.db")
    if err != nil {
        log.Fatal(err)
    }
    defer db.Close()

    // Create table
    _, err = db.Exec(`CREATE TABLE IF NOT EXISTS users (
        id INTEGER PRIMARY KEY AUTOINCREMENT,
        username TEXT,
        password TEXT
    )`)
    if err != nil {
        log.Fatal(err)
    }

    // Insert a test user
    _, err = db.Exec(`INSERT INTO users (username, password) VALUES ('admin', 'password')`)
    if err != nil {
        log.Fatal(err)
    }

    http.HandleFunc("/login", loginHandler)
    fmt.Println("Server is running at http://localhost:8080")
    log.Fatal(http.ListenAndServe(":8080", nil))
}

func loginHandler(w http.ResponseWriter, r *http.Request) {
    if r.Method != http.MethodPost {
        http.Error(w, "Method not allowed", http.StatusMethodNotAllowed)
        return
    }

    username := r.FormValue("username")
    password := r.FormValue("password")
    fmt.Println("username:", username)
    fmt.Println("password:", password)

    // Vulnerable query
    query := fmt.Sprintf("SELECT * FROM users WHERE username = '%s' AND password = '%s'", username, password)

    fmt.Println("running query:", query)

    var id int
    var dbUsername, dbPassword string
    err := db.QueryRow(query).Scan(&id, &dbUsername, &dbPassword)
    if err != nil {
        if err == sql.ErrNoRows {
            http.Error(w, "Invalid credentials", http.StatusUnauthorized)
            return
        }
	fmt.Println(err)
        http.Error(w, "Server error", http.StatusInternalServerError)
        return
    }

    fmt.Fprintf(w, "Welcome, %s!", dbUsername)
}

