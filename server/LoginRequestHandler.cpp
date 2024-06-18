#include "LoginRequestHandler.h"

#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"

LoginRequestHandler::LoginRequestHandler(RequestHandlerFactory& handleFactory) : m_handleFactory(handleFactory)
{
}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& request) const
{
    return request.id == MessageType::MT_CLIENT_LOG_IN || request.id == MessageType::MT_CLIENT_SIGN_UP;
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& request)
{
    RequestResult result;
    switch (request.id)
    {
    case MT_CLIENT_LOG_IN:
    {
        result = login(request);
        break;
    }
    case MT_CLIENT_SIGN_UP:
    {
        result = signup(request);
        break;
    }
    default:
    {
        ErrorResponse errorResponse;
        errorResponse.message = "Invalid request ID";

        result.response = JsonResponsePacketSerializer::serializeErrorResponse(errorResponse);
        result.newHandler = m_handleFactory.createLoginRequestHandler();
        break;
    }
    }

    return result;
}

void LoginRequestHandler::logOutUserIfCrash()
{
}

RequestResult LoginRequestHandler::login(const RequestInfo& request) const
{
    RequestResult result;
    try
    {
        LoginRequest loginRequest = JsonRequestPacketDeserializer::deserializeLoginRequest(request.buffer);
        LoginResponse loginResponse;

        loginResponse.status = (int)m_handleFactory.getLoginManager().login(loginRequest.username, loginRequest.password);

        result.response = JsonResponsePacketSerializer::serializeLoginResponse(loginResponse);

        if (loginResponse.status == (int)Login_Handler_Statuses::LOGIN_SUCCES)
        {
            result.newHandler = m_handleFactory.createMenuRequestHandler(LoggedUser(loginRequest.username));
        }
        else
        {
            result.newHandler = m_handleFactory.createLoginRequestHandler();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error with deserialize, error: " << e.what() << std::endl;
    }
    
    return result;
}

RequestResult LoginRequestHandler::signup(const RequestInfo& request) const
{
    RequestResult result;
    try
    {
        SignupRequest signupRequest = JsonRequestPacketDeserializer::deserializeSignUpRequest(request.buffer);
        SignupResponse signupResponse;

        signupResponse.status = (int)m_handleFactory.getLoginManager().signup(signupRequest.username, signupRequest.password, signupRequest.email);

        result.response = JsonResponsePacketSerializer::serializeSignUpResponse(signupResponse);

        if (signupResponse.status == (int)Login_Handler_Statuses::SIGN_UP_SUCCES)
        {
            result.newHandler = m_handleFactory.createMenuRequestHandler(LoggedUser(signupRequest.username));
        }
        else
        {
            result.newHandler = m_handleFactory.createLoginRequestHandler();
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error with deserialize, error: " << e.what() << std::endl;
    }
    
    return result;
}
