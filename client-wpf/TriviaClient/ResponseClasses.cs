using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TriviaClient
{
    public class LoginRespnose
    {
        public int status { get; set; }
    }

    public class SignupRespnose
    {
        public int status { get; set; }
    }

    public class SignOutRespnose
    {
        public int status { get; set; }
    }

    public class CreateRoomRespnose
    {
        public int status { get; set; }
    }

    public class JoinRoomRespnose
    {
        public int status { get; set; }
    }

    public class GetPersonalStatisticsResponse
    {
        public int status { get; set; }
        public PersonalStatistics statistics { get; set; }
    }

    public class PersonalStatistics
    {
        public float avarageAnswerTime;
        public int numOfCorrectAnswers;
        public int numOfTotalAnswers;
        public int numOfPlayerGames;
        public int playerScore;
    }

    public class GetHighScoresResponse
    {
        public List<HighScores> statistics { get; set; }
        public int status { get; set; }
    }

    public class HighScores
    {
        public int score { get; set; }
        public string username { get; set; }
    }

    public class GetPlayersInRoomRespnose
    {
        public List<string> players { get; set; }
    }

    public class GetRoomsResponse
    {
        public int status { get; set; }
        public List<Room> rooms { get; set; }
    }

    public class Room
    {
        public int id { get; set; }
        public int maxPlayers { get; set; }
        public string name { get; set; }
        public int numOfQuestionsInGame { get; set; }
        public int status { get; set; }
        public int timePerQuestion { get; set; }
    }

    public enum RoomState : int
    {
        DIDNT_START = 0,
        IN_GAME = 1,
        DOESNT_EXIST = 2,
        ROOM_CLOSE = 3
    };

    public class GetRoomStateResponse
    {
        public int status;
        public RoomState roomState;
        public List<String> players;
        public int questionCount;
        public int answerTimeout;

    }

    public class LeaveRoomResponse
    {
        public int status { get; set; }
    }
    
    public class StartGameResponse
    {
        public int status { get; set; }
    }

    public class CloseRoomResponse
    {
        public int status { get; set; }
    }

    public class SubmitAnswerResponse
    {
        public int status { get; set; }
        public int correctAnswerId { get; set; }
    }

    public class LeaveGameResponse
    {
        public int status { get; set; }
    }

    public class GetQuestionResponse
    {
        public int status { get; set; }
        public string question { get; set; }
        public Dictionary<int, string> answers { get; set; }
    }

    public class GetGameResultsResponse
    {
        public int status { get; set; }
        public List<PlayerResults> results { get; set; }
    }

    public class PlayerResults
    {
        public string username { get; set; }
        public int correctAnswerCount { get; set; }
        public int wrongAnswerCount { get; set; }
        public int averageAnswerTime { get; set; }
    }
}
