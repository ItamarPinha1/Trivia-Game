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
    /// Interaction logic for InWaitingRoomPage.xaml
    /// </summary>


    public partial class InWaitingRoomPage : Page
    {

        enum ResponseCode : int
        {
            LEAVE_ROOM_SUCCES = 131,
            LEAVE_ROOM_FAIL = 132,
        }

        private Communicator m_communicator;
        private Thread m_UpdateThread;
        private bool m_isRoomDataUpdateRunning;
        private bool m_isUpdateRunning;
        private int m_roomStatus;

        private CreateRoomRequest m_room;

        public InWaitingRoomPage(Communicator communicator, CreateRoomRequest room)
        {
            InitializeComponent();
            m_communicator = communicator;
            m_room = room;

            RoomNameTextBlock.Text = room.roomName;
            MaxUsersTextBlock.Text = room.maxUsers.ToString();
            QuestionCountTextBlock.Text = room.questionCount.ToString();
            AnswerTimeoutTextBlock.Text = room.answerTimeout.ToString();

            m_isUpdateRunning = true;
            m_UpdateThread = new Thread(() => UpdatePlayersList());
            m_UpdateThread.Start();

        }

        private void UpdatePlayersList()
        {
            while (m_isUpdateRunning)
            {
                byte[] responseBytes;
                MessageType messageType;
                SendGetRoomState(out responseBytes, out messageType);

                switch (messageType)
                {
                    case MessageType.GET_ROOM_STATE:
                        GetRoomStateResponse response = Desrializer.Deserialize<GetRoomStateResponse>(responseBytes);
                        printPlayers(response.players);
                        handleRoomState(response.roomState);
                        break;
                    default:
                        break;
                }

                Thread.Sleep(300);
            }
        }

        private void handleRoomState(RoomState roomState)
        {
            Application.Current.Dispatcher.Invoke(() =>
            {
                switch (roomState)
                {
                    case RoomState.ROOM_CLOSE:
                        NavigationService.Navigate(new MainMenu(m_communicator));
                        break;

                    case RoomState.IN_GAME:
                        NavigationService.Navigate(new GameScreen(m_communicator, m_room.answerTimeout, m_room.questionCount));
                        break;

                    case RoomState.DOESNT_EXIST:
                        NavigationService.Navigate(new MainMenu(m_communicator));
                        ErrorMessage.Text = "Room doesn't exist.";
                        break;

                    default:
                        break;
                }
            });
        }

        private void SendGetRoomState(out byte[] responseBytes, out MessageType messageType)
        {
            GetRoomStateRequest getRoomsReuquest = new GetRoomStateRequest();

            byte[] message = Serializer.Serialize<GetRoomStateRequest>(MessageType.GET_ROOM_STATE, getRoomsReuquest);

            m_communicator.sendMessage(message);

            responseBytes = m_communicator.recvMessage();
            messageType = (MessageType)responseBytes[0];
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
            m_isUpdateRunning = false;
        }

        private void LeavevGameButton_Click(object sender, RoutedEventArgs e)
        {
            LeaveRoomRequest leaveRoomRequest = new LeaveRoomRequest();

            byte[] message = Serializer.Serialize<LeaveRoomRequest>(MessageType.LEAVE_ROOM, leaveRoomRequest);

            m_communicator.sendMessage(message);

            byte[] responseBytes = m_communicator.recvMessage();

            MessageType messageType = (MessageType)responseBytes[0];

            switch (messageType)
            {
                case MessageType.LEAVE_ROOM:
                    LeaveRoomResponse response = Desrializer.Deserialize<LeaveRoomResponse>(responseBytes);
                    handleLeaveRoom(response.status);
                    break;
                default:
                    break;
            }
        }

        private void handleLeaveRoom(int status)
        {
            switch ((ResponseCode)status) 
            {
                case ResponseCode.LEAVE_ROOM_SUCCES:
                    NavigationService.Navigate(new MainMenu(m_communicator));
                    break;

            }
        }
    }
}
