import sqlite3
import requests

url = 'https://opentdb.com/api.php?amount=20&type=multiple'
headers = {'User-Agent': 'My-App/0.0.1'}
params = {'key': 'value'}

response = requests.get(url, headers=headers, params=params)

if response.status_code == 200:
    print('Request successful!')
    # Parse JSON response
    data = response.json()
    questions = data['results']
else:
    print('Request failed with status code:', response.status_code)

# Connect to SQLite database (creates it if it doesn't exist)
conn = sqlite3.connect('triviaDB.sqlite')
# Create a cursor object to execute SQL commands
cursor = conn.cursor()
# Insert questions into the table
for question in questions:
    cursor.execute("INSERT INTO QUESTIONS (QUESTION, DIFFICULTY, CATEGORY, [CORRECT-ANSWER], [ANSWER-A], [ANSWER-B], [ANSWER-C]) VALUES (?, ?, ?, ?, ?, ?, ?)", (question['question'], question['difficulty'], question['category'], question['correct_answer'], question['incorrect_answers'][0], question['incorrect_answers'][1], question['incorrect_answers'][2]))

# save changes to the database
conn.commit()

# Close the cursor and connection
cursor.close()
conn.close()