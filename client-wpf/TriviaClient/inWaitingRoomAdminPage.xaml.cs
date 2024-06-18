using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for inWaitingRoomAdminPage.xaml
    /// </summary>
    public partial class inWaitingRoomAdminPage : Page
    {

        enum ResponseCode : int
        {
            CLOSE_ROOM_SUCCES = 101,
            CLOSE_ROOM_FAIL = 102,

            START_GAME_SUCCES = 111,
            START_GAME_FAIL = 112,
        }

        private Communicator m_communicator;
        private Thread m_playerUpdateThread;
        private bool m_isPlayerUpdateRunning;

        private CreateRoomRequest m_room;



        public inWaitingRoomAdminPage(Communicator communicator, CreateRoomRequest room)
        {
            InitializeComponent();
            m_communicator = communicator;
            m_room = room;

            RoomNameTextBlock.Text = room.roomName;
            MaxUsersTextBlock.Text = room.maxUsers.ToString();
            QuestionCountTextBlock.Text = room.questionCount.ToString();
            AnswerTimeoutTextBlock.Text = room.answerTimeout.ToString();

            m_isPlayerUpdateRunning = true;
            m_playerUpdateThread = new Thread(() => UpdatePlayersList());
            m_playerUpdateThread.Start();



        }

        private void UpdatePlayersList()
        {
            while (m_isPlayerUpdateRunning)
            {
                handleGetPlayers();

                Thread.Sleep(3000);
            }
        }

        public void handleGetPlayers()
        {
            GetRoomStateRequest getRoomsReuquest = new GetRoomStateRequest();

            byte[] message = Serializer.Serialize<GetRoomStateRequest>(MessageType.GET_ROOM_STATE, getRoomsReuquest);

            m_communicator.sendMessage(message);

            byte[] responseBytes = m_communicator.recvMessage();

            MessageType messageType = (MessageType)responseBytes[0];

            switch (messageType)
            {
                case MessageType.GET_ROOM_STATE:
                    GetRoomStateResponse response = Desrializer.Deserialize<GetRoomStateResponse>(responseBytes);
                    printPlayers(response.players);
                    break;
                default:
                    break;
            }
        }
        
        public void printPlayers(List<string> playerNames)
        {
            try
            {
                Application.Current.Dispatcher.Invoke(() =>
                {
                    // Clear existing player names
                    PlayerListStackPanel.Children.Clear();

                    // Add each player name to the stack panel
                    foreach (string playerName in playerNames)
                    {
                        TextBlock playerNameTextBlock = new TextBlock();
                        playerNameTextBlock.Text = playerName;
                        playerNameTextBlock.FontSize = 20;
                        playerNameTextBlock.Foreground = Brushes.White;
                        playerNameTextBlock.Margin = new Thickness(0, 0, 0, 10); // Add margin for spacing
                        playerNameTextBlock.HorizontalAlignment = HorizontalAlignment.Center;
                        PlayerListStackPanel.Children.Add(playerNameTextBlock);
                    }
                });
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error printing player names: " + ex.Message);
            }
        }

        private void Page_Unloaded(object sender, RoutedEventArgs e)
        {
            m_isPlayerUpdateRunning = false;
            m_playerUpdateThread.Join();
        }

        private void CloseRoomButton_Click(object sender, RoutedEventArgs e)
        {
            CloseRoomRequest closeRoomRequest = new CloseRoomRequest();

            byte[] message = Serializer.Serialize<CloseRoomRequest>(MessageType.CLOSE_ROOM, closeRoomRequest);

            m_communicator.sendMessage(message);

            byte[] responseBytes = m_communicator.recvMessage();

            MessageType messageType = (MessageType)responseBytes[0];

            switch (messageType)
            {
                case MessageType.CLOSE_ROOM:
                    CloseRoomResponse response = Desrializer.Deserialize<CloseRoomResponse>(responseBytes);
                    handleCloseRoom(response.status);
                    break;
                default:
                    break;
            }
        }

        private void handleCloseRoom(int status)
        {
            switch((ResponseCode)status)
            {
                case ResponseCode.CLOSE_ROOM_SUCCES:
                    NavigationService.Navigate(new MainMenu(m_communicator));
                    break;

                }
            }


        private void StartGameButton_Click(object sender, RoutedEventArgs e)
        {
            StartGameRequest startGameRequest = new StartGameRequest();

            byte[] message = Serializer.Serialize<StartGameRequest>(MessageType.START_GAME, startGameRequest);

            m_communicator.sendMessage(message);

            byte[] responseBytes = m_communicator.recvMessage();

            MessageType messageType = (MessageType)responseBytes[0];

            switch (messageType)
            {
                case MessageType.START_GAME:
                    StartGameResponse response = Desrializer.Deserialize<StartGameResponse>(responseBytes);
                    handleStartGame(response.status);
                    break;
                default:
                    break;
            }
        }

        private void handleStartGame(int status)
        {
            switch ((ResponseCode)status)
            {
                case ResponseCode.START_GAME_SUCCES:
                    NavigationService.Navigate(new GameScreen(m_communicator, m_room.answerTimeout, m_room.questionCount));
                    break;

            }
        }
    }
}
