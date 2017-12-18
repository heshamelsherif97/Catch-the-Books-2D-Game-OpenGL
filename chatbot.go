package main

import (
	"crypto/md5"
	"encoding/hex"
	"encoding/json"
	"fmt"
	"log"
	"os"
	"net/http"
	"net/http/httptest"
	"strconv"
	"unicode"
	"strings"
	"time"

	cors "github.com/heppu/simple-cors"
)

import _ "github.com/joho/godotenv/autoload"

   type Symptom struct {
			ID int
    	NAME string
    }

		type Issue struct {
			ID int
			NAME string
			ProfName string
			Icd string
			IcdName string
			Accuracy int
		}
    

   type Specialisation struct {
			ID int
    	NAME string
		 	SpecialistID int
    }

   type Diagnose struct {
		 	Issue Issue
		 	Specialisation [] Specialisation
    }

		type Token struct {
    	Token string
			ValidThrough int
		}	

var (
	// WelcomeMessage A constant to hold the welcome message
	WelcomeMessage = "Welcome, what is your name?"

	// sessions = {
	//   "uuid1" = Session{...},
	//   ...
	// }
	sessions = map[string]Session{}
	language = "en-gb"
	allSymptoms [] Symptom

	
	processor = sampleProcessor
)

type (
	// Session Holds info about a session
	Session map[string]interface{}

	// JSON Holds a JSON object
	JSON map[string]interface{}

	// Processor Alias for Process func
	Processor func(session Session, message string) (string, error)
)

//Checks string contains alphabets
func IsLetter(s string) bool {
    for _, r := range s {
        if !unicode.IsLetter(r) {
            return false
        }
    }
    return true
}

//checks string is a number
func isInt(s string) bool {
    for _, c := range s {
        if !unicode.IsDigit(c) {
            return false
        }
    }
    return true
}



func sampleProcessor(session Session, message string) (string, error) {
	if(session["counter"] == nil){
		session["counter"] = 0
	}
	if(session["proposedCounter"] == nil){
		session["proposedCounter"] = 0
	}
	counter, _ := session["counter"].(int)
	proposedCounter, _ := session["proposedCounter"].(int)
	if session["stage"] == nil  {
		
		if(IsLetter(message)){
			session["name"] = message
			session["stage"] = "gender"
			return "Hello "+message+"! What's your gender?" , nil
		}else{
			return "This is not a correct name rewrite your name Please!" , nil
		}
	}else if session["stage"] == "gender"{
		if(strings.Contains(strings.ToLower(message), "female") || strings.Contains(strings.ToLower(message), "woman")){
			session["stage"] = "birth"
			session["gender"] = "female"
			return "So you are a female, Can you tell me your year of birth?", nil
		}else if(strings.Contains(strings.ToLower(message), "male") || strings.Contains(strings.ToLower(message), "man")){
			session["stage"] = "birth"
			session["gender"] = "male"
			return "So you are a male, Can you tell me your year of birth?", nil
		}else{
			return "I am sorry I can't understand you, Are you male or female?", nil
		}
	}else if session["stage"] == "birth"{
		if(isInt(message)){
			i, _ := strconv.Atoi(message)
			if(i > 2017 || i < 1900){
				return "Please Enter a valid date", nil
			}else{
				session["stage"] = "symptoms"
				session["yearOfBirth"] = message
				return "What symptoms are you feeling?", nil
			}
		}else{
			return "Wow, This can't be a year", nil
		}
	}else if session["stage"] == "symptoms" && counter == 0{
		if checkSymptom(message, session) {
			session["stage"] = "decision"
			return "Do you think you have a "+getPropsedSymptoms(proposedCounter, session)+"?", nil
		}else{
			return "We don't have that Symptom in our database, Please check your spelling", nil
		}
	}else if session["stage"] == "decision" && counter < 4 {
		if strings.ToLower(message) == "yes" || strings.ToLower(message) == "yeah" {
			counter++
			session["counter"] = counter
			session["clientSymptoms"] = append(session["clientSymptoms"].([]int), session["id"].(int))
			fmt.Println(session["clientSymptoms"].([]int))
			return "Do you think you have a "+getPropsedSymptoms(proposedCounter, session)+"?", nil
		}else if strings.ToLower(message) == "no" || strings.ToLower(message) == "nope" {
			counter ++
			session["counter"] =counter
			proposedCounter ++
			session["proposedCounter"] = proposedCounter
			return "Do you think you have a "+getPropsedSymptoms(proposedCounter, session)+"?", nil
		}else {
			return "Please answer with Yes or No", nil
		}
	}else if session["stage"] == "decision" && counter == 4 {
		session["stage"] = "diagnosis"
		getDiagnosis(session);
		return "Our diagnosis shows that you might have "+ session["diagnosis"].([]Diagnose)[0].Issue.NAME +"/"+ session["diagnosis"].([]Diagnose)[0].Issue.ProfName +", Your problem is "+ session["diagnosis"].([]Diagnose)[0].Specialisation[0].NAME +" related, Do you want to have another check?", nil
	}else if session["stage"] == "diagnosis" {
		if strings.ToLower(message) == "yes" || strings.ToLower(message) == "yeah" {
			session["stage"] = "symptoms"
			session["clientSymptoms"] = make([]int, 0)
			counter = 0
			proposedCounter = 0
			session["counter"] =counter
			session["proposedCounter"] = proposedCounter
			return "What symptoms are you feeling?", nil
		}else if strings.ToLower(message) == "no" || strings.ToLower(message) == "nope" {
			session["stage"] = ""
			return "Thank you for using the chatBot", nil
		}else{
			return "Please answer with Yes or No", nil
		}
	}else if session["stage"] == "end" {
		session["stage"] = "diagnosis"
		return "Do you want to have another check?", nil
	}
	session["stage"] = "end"
	return "Our propsed issues are not 100% accurate, It's always better to be examined by a doctor", nil
}

//check the symptom
func checkSymptom(s string, session Session)(bool){
	var clientSymptoms [] int
	for i:=0; i < len(allSymptoms); i++{
		if(strings.ToLower(s) == strings.ToLower(allSymptoms[i].NAME)){
			clientSymptoms = append(clientSymptoms, allSymptoms[i].ID)
			session["clientSymptoms"] = clientSymptoms
			return true
		}
	}
	return false
}

//Get All symptoms
func getSymptoms(session Session){
	r, _ := http.Get("https://sandbox-healthservice.priaid.ch/symptoms?language="+language+"&token="+session["token"].(Token).Token)
	defer r.Body.Close()
	json.NewDecoder(r.Body).Decode(&allSymptoms)
}

//Diagnose
func getDiagnosis(session Session){
	symptoms := "["+strings.Trim(strings.Replace(fmt.Sprint(session["clientSymptoms"].([]int)), " ", ",", -1), "[]")+"]"
	r, _ := http.Get("https://sandbox-healthservice.priaid.ch/diagnosis?language="+language+"&token="+session["token"].(Token).Token+"&gender="+session["gender"].(string)+"&symptoms="+symptoms+"&year_of_birth="+session["yearOfBirth"].(string))
	defer r.Body.Close()
	var diagnosis [] Diagnose
	json.NewDecoder(r.Body).Decode(&diagnosis)
	session["diagnosis"] = diagnosis
}

func getPropsedSymptoms(x int, session Session)(s string){
	symptoms := "["+strings.Trim(strings.Replace(fmt.Sprint(session["clientSymptoms"].([]int)), " ", ",", -1), "[]")+"]"
	r, _ := http.Get("https://sandbox-healthservice.priaid.ch/symptoms/proposed?language="+language+"&token="+session["token"].(Token).Token+"&gender="+session["gender"].(string)+"&symptoms="+symptoms+"&year_of_birth="+session["yearOfBirth"].(string))
	defer r.Body.Close()
	var proposed [] Symptom
	json.NewDecoder(r.Body).Decode(&proposed)
	if(x >= len(proposed)){
		session["id"] = proposed[0].ID
		return proposed[0].NAME	
	}else{
		session["id"] = proposed[x].ID
		return proposed[x].NAME		
	}								
}

func getToken(session Session){
	client := &http.Client{}
	var auth = "Bearer heshamelsherif97@gmail.com:VKyFZCXs6qEwZvl2Yq5Cgw=="
	req, _ := http.NewRequest("POST", "https://sandbox-authservice.priaid.ch/login", nil)
	req.Header.Add("Authorization", auth)
	resp, _ := client.Do(req)
	var token Token
	json.NewDecoder(resp.Body).Decode(&token)
	session["token"] = token
}

// withLog Wraps HandlerFuncs to log requests to Stdout
func withLog(fn http.HandlerFunc) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		c := httptest.NewRecorder()
		fn(c, r)
		log.Printf("[%d] %-4s %s\n", c.Code, r.Method, r.URL.Path)

		for k, v := range c.HeaderMap {
			w.Header()[k] = v
		}
		w.WriteHeader(c.Code)
		c.Body.WriteTo(w)
	}
}

// writeJSON Writes the JSON equivilant for data into ResponseWriter w
func writeJSON(w http.ResponseWriter, data JSON) {
	w.Header().Set("Content-Type", "application/json")
	json.NewEncoder(w).Encode(data)
}

// ProcessFunc Sets the processor of the chatbot
func ProcessFunc(p Processor) {
	processor = p
}

// handleWelcome Handles /welcome and responds with a welcome message and a generated UUID
func handleWelcome(w http.ResponseWriter, r *http.Request) {
	// Generate a UUID.
	hasher := md5.New()
	hasher.Write([]byte(strconv.FormatInt(time.Now().Unix(), 10)))
	uuid := hex.EncodeToString(hasher.Sum(nil))
	// Create a session for this UUID
	sessions[uuid] = Session{}
	getToken(sessions[uuid])
	getSymptoms(sessions[uuid])
	// Write a JSON containg the welcome message and the generated UUID
	writeJSON(w, JSON{
		"uuid":    uuid,
		"message": WelcomeMessage,
	})
}

func handleChat(w http.ResponseWriter, r *http.Request) {
	// Make sure only POST requests are handled
	if r.Method != http.MethodPost {
		http.Error(w, "Only POST requests are allowed.", http.StatusMethodNotAllowed)
		return
	}

	// Make sure a UUID exists in the Authorization header
	uuid := r.Header.Get("Authorization")
	if uuid == "" {
		http.Error(w, "Missing or empty Authorization header.", http.StatusUnauthorized)
		return
	}

	// Make sure a session exists for the extracted UUID
	session, sessionFound := sessions[uuid]
	if !sessionFound {
		http.Error(w, fmt.Sprintf("No session found for: %v.", uuid), http.StatusUnauthorized)
		return
	}

	// Parse the JSON string in the body of the request
	data := JSON{}
	if err := json.NewDecoder(r.Body).Decode(&data); err != nil {
		http.Error(w, fmt.Sprintf("Couldn't decode JSON: %v.", err), http.StatusBadRequest)
		return
	}
	defer r.Body.Close()

	// Make sure a message key is defined in the body of the request
	_, messageFound := data["message"]
	if !messageFound {
		http.Error(w, "Missing message key in body.", http.StatusBadRequest)
		return
	}

	// Process the received message
	message, err := processor(session, data["message"].(string))
	if err != nil {
		http.Error(w, err.Error(), 422 /* http.StatusUnprocessableEntity */)
		return
	}

	// Write a JSON containg the processed response
	writeJSON(w, JSON{
		"message": message,
	})
}

// handle Handles /
func handle(w http.ResponseWriter, r *http.Request) {
	body :=
		"<!DOCTYPE html><html><head><title>Chatbot</title></head><body><pre style=\"font-family: monospace;\">\n" +
			"Available Routes:\n\n" +
			"  GET  /welcome -> handleWelcome\n" +
			"  POST /chat    -> handleChat\n" +
			"  GET  /        -> handle        (current)\n" +
			"</pre></body></html>"
	w.Header().Add("Content-Type", "text/html")
	fmt.Fprintln(w, body)
}

// Engage Gives control to the chatbot
func Engage(addr string) error {
	// HandleFuncs
	mux := http.NewServeMux()
	mux.HandleFunc("/welcome", withLog(handleWelcome))
	mux.HandleFunc("/chat", withLog(handleChat))
	mux.HandleFunc("/", withLog(handle))

	// Start the server
	return http.ListenAndServe(addr, cors.CORS(mux))
}
func main() {
	port := os.Getenv("PORT")
	// Default to 3000 if no PORT environment variable was defined
	if port == "" {
		port = "3000"
	}

	// Start the server
	fmt.Printf("Listening on port %s...\n", port)
	log.Fatalln(Engage(":" + port))
}
