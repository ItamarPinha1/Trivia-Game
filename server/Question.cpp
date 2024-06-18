#include "Question.h"
#include <algorithm>
#include <random>

Question::Question(const std::string& question, const std::string& correctAnswer, std::vector<std::string>& possibleAnswers)
{
	this->m_question = question;
	this->m_correctAnswer = correctAnswer;
	setPossibleAnswers(possibleAnswers);
}

void Question::setPossibleAnswers(std::vector<std::string>& possibleAnswers)
{
	// set the possible answers in a random order
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(possibleAnswers.begin(), possibleAnswers.end(), g);

	this->m_possibleAnswers = possibleAnswers;
}

std::string Question::getQuestion()
{
    return m_question;
}

std::vector<std::string> Question::getPossibleAnswers()
{
    return m_possibleAnswers;
}

int Question::getCorrectAnswerId()
{
	for (int i = 0; i < m_possibleAnswers.size(); i++)
	{
		if (m_possibleAnswers[i] == m_correctAnswer)
		{
			return i;
		}
	}
	return -1;
}
