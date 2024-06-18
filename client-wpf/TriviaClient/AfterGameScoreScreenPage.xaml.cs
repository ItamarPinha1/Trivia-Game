using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using static System.Formats.Asn1.AsnWriter;

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for AfterGameScoreScreenPage.xaml
    /// </summary>
    public partial class AfterGameScoreScreenPage : Page
    {

        enum ResponseCode : int
        {
            LEAVE_SUCESS = 141,
            LEAVE_FAIL = 142,
        }

        private Communicator m_communicator;

        public AfterGameScoreScreenPage(Communicator communicator)
        {
            InitializeComponent();

            m_communicator = communicator;
            UpdatePlayerScores();
        }

        public void UpdatePlayerScores()
        {
            List<PlayerResults> playersList;
            do 
            {
                playersList = GameScores();
            } while (playersList == null);

            

            int numPlayers = playersList.Count();

            for (int i = 0; i < numPlayers; i++)
            {
                // Update players ranks
                if (i == 0)
                    player1Rank.Text = "1";
                else if (i == 1)
                    player2Rank.Text = "2";
                else if (i == 2)
                    player3Rank.Text = "3";
                else if (i == 3)
                    player4Rank.Text = "4";
                else if (i == 4)
                    player5Rank.Text = "5";
                else if (i == 5)
                    player6Rank.Text = "6";
                else if (i == 6)
                    player7Rank.Text = "7";
                else if (i == 7)
                    player8Rank.Text = "8";
                // Update player names
                if (i == 0)
                    player1Name.Text = playersList[i].username;
                else if (i == 1)
                    player2Name.Text = playersList[i].username;
                else if (i == 2)
                    player3Name.Text = playersList[i].username;
                else if (i == 3)
                    player4Name.Text = playersList[i].username;
                else if (i == 4)
                    player5Name.Text = playersList[i].username;
                else if (i == 5)
                    player6Name.Text = playersList[i].username;
                else if (i == 6)
                    player7Name.Text = playersList[i].username;
                else if (i == 7)
                    player8Name.Text = playersList[i].username;

                // Update correct answers count
                if (i == 0)
                    player1CorrectAnswers.Text = playersList[i].correctAnswerCount.ToString();
                else if (i == 1)
                    player2CorrectAnswers.Text = playersList[i].correctAnswerCount.ToString();
                else if (i == 2)
                    player3CorrectAnswers.Text = playersList[i].correctAnswerCount.ToString();
                else if (i == 3)
                    player4CorrectAnswers.Text = playersList[i].correctAnswerCount.ToString();
                else if (i == 4)
                    player5CorrectAnswers.Text = playersList[i].correctAnswerCount.ToString();
                else if (i == 5)
                    player6CorrectAnswers.Text = playersList[i].correctAnswerCount.ToString();
                else if (i == 6)
                    player7CorrectAnswers.Text = playersList[i].correctAnswerCount.ToString();
                else if (i == 7)
                    player8CorrectAnswers.Text = playersList[i].correctAnswerCount.ToString();

                // Update time per answer
                if (i == 0)
                    player1TimePerAnswer.Text = playersList[i].averageAnswerTime.ToString("0.00") + "s";
                else if (i == 1)
                    player2TimePerAnswer.Text = playersList[i].averageAnswerTime.ToString("0.00") + "s";
                else if (i == 2)
                    player3TimePerAnswer.Text = playersList[i].averageAnswerTime.ToString("0.00") + "s";
                else if (i == 3)
                    player4TimePerAnswer.Text = playersList[i].averageAnswerTime.ToString("0.00") + "s";
                else if (i == 4)
                    player5TimePerAnswer.Text = playersList[i].averageAnswerTime.ToString("0.00") + "s";
                else if (i == 5)
                    player6TimePerAnswer.Text = playersList[i].averageAnswerTime.ToString("0.00") + "s";
                else if (i == 6)
                    player7TimePerAnswer.Text = playersList[i].averageAnswerTime.ToString("0.00") + "s";
                else if (i == 7)
                    player8TimePerAnswer.Text = playersList[i].averageAnswerTime.ToString("0.00") + "s";
            }
        }

        


        private List<PlayerResults> GameScores()
        {
            GetGameResultsRequest gameResultsRequest = new GetGameResultsRequest();

            byte[] message = Serializer.Serialize<GetGameResultsRequest>(MessageType.GET_GAME_RESULTS, gameResultsRequest);

            m_communicator.sendMessage(message);

            byte[] responseBytes = m_communicator.recvMessage();

            MessageType messageType = (MessageType)responseBytes[0];

            switch (messageType)
            {
                case MessageType.GET_GAME_RESULTS:
                    GetGameResultsResponse response = Desrializer.Deserialize<GetGameResultsResponse>(responseBytes);
                    return response.results;
                default:
                    return null;
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            LeaveGameRequest request = new LeaveGameRequest();

            byte[] message = Serializer.Serialize<LeaveGameRequest>(MessageType.LEAVE_GAME, request);

            m_communicator.sendMessage(message);

            byte[] responseBytes = m_communicator.recvMessage();

            MessageType messageType = (MessageType)responseBytes[0];

            switch (messageType)
            {
                case MessageType.LEAVE_GAME:
                    LeaveGameResponse response = Desrializer.Deserialize<LeaveGameResponse>(responseBytes);
                    handleLeaveGame(response.status);
                    break;
                default:
                    return;
            }
        }

        public void handleLeaveGame(int status)
        {
            switch ((ResponseCode)status)
            {
                case ResponseCode.LEAVE_SUCESS:
                    NavigationService.Navigate(new MainMenu(m_communicator));
                    break;

                default:
                    break;
            }
        }
    }
}
