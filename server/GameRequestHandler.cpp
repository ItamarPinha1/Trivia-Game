#include "GameRequestHandler.h"

#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"

GameRequestHandler::GameRequestHandler(Game& game, const LoggedUser& user, GameManager& gameManager, RequestHandlerFactory& factory)
    : m_game(game), m_user(user), m_gameManager(gameManager), m_handleFactory(factory)
{
}

bool GameRequestHandler::isRequestRelevant(const RequestInfo& request) const
{
    return request.id == MessageType::MT_CLIENT_GET_GAME_RESULTS || 
        request.id == MessageType::MT_CLIENT_GET_QUESTION || 
        request.id == MessageType::MT_CLIENT_LEAVE_GAME || 
        request.id == MessageType::MT_CLIENT_SUBMIT_ANSWER;
}

RequestResult GameRequestHandler::handleRequest(const RequestInfo& request)
{
    RequestResult result;
    switch (request.id)
    {
    case MT_CLIENT_GET_GAME_RESULTS:
    {
        result = getGameResults(request);
        break;
    }
    case MT_CLIENT_GET_QUESTION:
    {
        result = getQuestion(request);
        break;
    }
    case MT_CLIENT_LEAVE_GAME:
    {
        result = leaveGame(request);
        break;
    }
    case MT_CLIENT_SUBMIT_ANSWER:
    {
        result = submitAnswer(request);
        break;
    }
    default:
    {
        ErrorResponse errorResponse;
        errorResponse.message = "Invalid request ID";

        result.response = JsonResponsePacketSerializer::serializeErrorResponse(errorResponse);
        result.newHandler = m_handleFactory.createGameRequestHandler(m_game, m_user);
        break;
    }
    }

    return result;
}

void GameRequestHandler::logOutUserIfCrash()
{
}

RequestResult GameRequestHandler::getQuestion(const RequestInfo& request) const
{
    RequestResult result;

    try
    {

        GetQuestionResponse getQuestionResponse;

        Question question = m_game.getQuestionForUser(m_user);

        if (question.getQuestion() == "")
        {
            getQuestionResponse.status = (int)Game_Statueses::GAME_FINISHED;
        }
        else
        {
            getQuestionResponse.status = (int)Game_Statueses::GET_QUESTION_SUCCES;
            
            getQuestionResponse.question = question.getQuestion();

            std::map<unsigned int, std::string> answersMap;
            auto answers = question.getPossibleAnswers();

            for (int i = 0; i < Answers_Numbers::NUMBER_OF_QUESTIONS; i++)
            {
                answersMap[i] = answers[i];
            }

            getQuestionResponse.answers = answersMap;
        }



        result.response = JsonResponsePacketSerializer::serializeGetQuestionResponse(getQuestionResponse);
        result.newHandler = m_handleFactory.createGameRequestHandler(m_game, m_user);

    }
    catch (const std::exception& e)
    {
        std::cerr << "Error with deserialize, error: " << e.what() << std::endl;

        ErrorResponse errorResponse;
        errorResponse.message = "Room doesnt Exist";

        result.newHandler = m_handleFactory.createGameRequestHandler(m_game, m_user);
        result.response = JsonResponsePacketSerializer::serializeErrorResponse(errorResponse);

    }

    return result;
}

RequestResult GameRequestHandler::submitAnswer(const RequestInfo& request) const
{
    RequestResult result;

    try
    {
        SubmitAnswerRequest submitAnswerRequest = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(request.buffer);
        
        SubmitAnswerResponse submitAnswerResponse;
        
        m_game.submitAnswer(m_user, submitAnswerRequest.answerId);

        submitAnswerResponse.correctAnswerId = m_game.getQuestionForUser(m_user).getCorrectAnswerId();
        submitAnswerResponse.status = (int)Game_Statueses::SUBMIT_ANSWER_SUCCES;

        result.response = JsonResponsePacketSerializer::serializeSubmitAnswerResponse(submitAnswerResponse);
        result.newHandler = m_handleFactory.createGameRequestHandler(m_game, m_user);

    }
    catch (const std::exception& e)
    {
        std::cerr << "Error with deserialize, error: " << e.what() << std::endl;

        ErrorResponse errorResponse;
        errorResponse.message = "Room doesnt Exist";

        result.newHandler = m_handleFactory.createGameRequestHandler(m_game, m_user);
        result.response = JsonResponsePacketSerializer::serializeErrorResponse(errorResponse);

    }

    return result;
}

RequestResult GameRequestHandler::getGameResults(const RequestInfo& request) const
{
    RequestResult result;

    try
    {

        GetGameResultsResponse gameResult;

        gameResult.results = m_handleFactory.getGameManager().getGameResults(m_game.getGameId());
        gameResult.status = (int)Game_Statueses::GET_GAME_RESULTS_SUCCES;

        result.response = JsonResponsePacketSerializer::serializeGetGameResultsResponse(gameResult);
        result.newHandler = m_handleFactory.createGameRequestHandler(m_game, m_user);
        
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error with deserialize, error: " << e.what() << std::endl;

        ErrorResponse errorResponse;
        errorResponse.message = "Room doesnt Exist";

        result.newHandler = m_handleFactory.createGameRequestHandler(m_game, m_user);
        result.response = JsonResponsePacketSerializer::serializeErrorResponse(errorResponse);

    }

    return result;
}

RequestResult GameRequestHandler::leaveGame(const RequestInfo& request) const
{
    RequestResult result;

    try
    {
        LeaveGameResponse leaveGameResponse;

        m_game.removeUser(m_user, m_handleFactory.getDB());

        leaveGameResponse.status = (int)Game_Statueses::LEAVE_SUCESS;

        result.response = JsonResponsePacketSerializer::serializeLeaveGameResponse(leaveGameResponse);
        result.newHandler = m_handleFactory.createMenuRequestHandler(m_user);

        if (m_game.getPlayers().size() == 0)
        {
            m_handleFactory.getRoomManager().deleteRoom(m_game.getGameId());
            m_gameManager.deleteGame(m_game.getGameId());
        }

    }
    catch (const std::exception& e)
    {
        std::cerr << "Error with deserialize, error: " << e.what() << std::endl;

        ErrorResponse errorResponse;
        errorResponse.message = "Room doesnt Exist";

        result.newHandler = m_handleFactory.createGameRequestHandler(m_game, m_user);
        result.response = JsonResponsePacketSerializer::serializeErrorResponse(errorResponse);

    }

    return result;
}


