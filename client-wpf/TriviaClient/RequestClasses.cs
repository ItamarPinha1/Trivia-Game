using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TriviaClient
{
    public class LoginRequest
    {
        public string username { get; set; }
        public string password { get; set; }
    }

    public class SignupRequest
    {
        public string username { get; set; }
        public string password { get; set; }
        public string email { get; set; }
    }

    public class SignOutRequest
    {

    }

    public class CreateRoomRequest
    {
        public string roomName { get; set; }
        public int maxUsers { get; set; }
        public int questionCount { get; set; }
        public int answerTimeout { get; set; }
    }

    public class JoinRoomRequest
    {
        public int roomId { get; set; }
    }

    public class GetPlayersInRoomRequest
    {
        public int roomId { get; set; }
    }

    public class GetRoomsReuquest
    {

    }

    public class GetHighScoresRequest
    {

    }

    public class GetPersonalStatisticsRequest
    {

    }

    public class GetRoomStateRequest
    {

    }

    public class LeaveRoomRequest
    {

    }
    public class StartGameRequest
    {

    }

    public class CloseRoomRequest
    {

    }

    public class SubmitAnswerRequest
    {
        public int answerId { get; set; }
    }

    public class LeaveGameRequest
    {

    }

    public class GetQuestionRequest
    {

    }

    public class GetGameResultsRequest
    {

    }
}
