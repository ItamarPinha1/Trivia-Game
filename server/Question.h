#pragma once
#include <string>
#include <vector>

class Question
{
private:


	std::string m_question;
	std::vector<std::string> m_possibleAnswers;
	std::string m_correctAnswer;

	void setPossibleAnswers(std::vector<std::string>& possibleAnswers);
public:
	
	Question() = default;
	
	Question(const std::string& question, const std::string& correctAnswer, std::vector<std::string>& possibleAnswers);
	std::string getQuestion();
	std::vector<std::string> getPossibleAnswers();
	int getCorrectAnswerId();
};

