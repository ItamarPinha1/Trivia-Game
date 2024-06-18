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
using System.Text.Json;
using System.Text.Json.Serialization;

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for JoinRoomPage.xaml
    /// </summary>
    public partial class JoinRoomPage : Page
    {

        enum ResponseCode : int
        {
            GET_ROOMS_SUCCESS = 51,
            GET_ROOMS_FAIL = 52,

            JOIN_ROOM_SUCCESS = 71,
            JOIN_ROOM_FAIL = 72,
            ROOM_NOT_FOUND = 43,
            USER_ALREADY_IN_ROOM = 46,
            ROOM_IS_FULL = 47,
        }

        private Communicator m_communicator;
        private Thread m_updateThread;
        private bool m_isRunning;

        public JoinRoomPage(Communicator communicator)
        {
            InitializeComponent();
            m_communicator = communicator;

            m_isRunning = true;
            m_updateThread = new Thread(UpdateRoomsList);
            m_updateThread.Start();

        }

        private void JoinRoomButton_Click(object sender, RoutedEventArgs e)
        {
            //NavigationService?.Navigate(new InWaitingRoomPage(m_communicator));
        }

        private void UpdateRoomsList()
        {
            while (m_isRunning)
            {
                GetRoomsReuquest getRoomsReuquest = new GetRoomsReuquest();

                byte[] message = Serializer.Serialize<GetRoomsReuquest>(MessageType.GET_ROOMS, getRoomsReuquest);

                m_communicator.sendMessage(message);

                byte[] responseBytes = m_communicator.recvMessage();

                MessageType messageType = (MessageType)responseBytes[0];

                switch (messageType)
                {
                    case MessageType.GET_ROOMS:
                        GetRoomsResponse response = Desrializer.Deserialize<GetRoomsResponse>(responseBytes);
                        handleGetRooms(response.status, response.rooms);
                        break;
                    default:
                        break;
                }

                Thread.Sleep(3000);
            }
        }

        public void handleGetRooms(int status, List<Room> rooms)
        {
            switch ((ResponseCode)status)
            {
                case ResponseCode.GET_ROOMS_SUCCESS:
                    UpdateUIWithRooms(rooms);
                    break;

                case ResponseCode.GET_ROOMS_FAIL:
                    break;

                default:
                    break;
            }
        }

        private void UpdateUIWithRooms(List<Room> rooms)
        {

            try
            {

                Application.Current.Dispatcher.Invoke(() =>
                {
                    RoomListBox.Items.Clear();

                    foreach (Room room in rooms)
                    {
                        if((RoomState)room.status == RoomState.DIDNT_START)
                        {
                             RoomListBox.Items.Add(room);
                        }
                    }
                });

            }
            catch (JsonException ex)
            {
                // Handle deserialization error
                Console.WriteLine("Error deserializing JSON: " + ex.Message);
            }

        }

        private void Page_Unloaded(object sender, RoutedEventArgs e)
        {
            m_isRunning = false;
            m_updateThread.Join();
        }

        public void JoinRoom_Click(object sender, RoutedEventArgs e)
        {
            Button button = sender as Button;
            if (button != null)
            {
                Room room = button.CommandParameter as Room;
                int roomId = room.id;

                JoinRoomRequest joinRoomRequest = new JoinRoomRequest();
                joinRoomRequest.roomId = roomId;

                byte[] message = Serializer.Serialize<JoinRoomRequest>(MessageType.JOIN_ROOM, joinRoomRequest);
                m_communicator.sendMessage(message);

                byte[] responseBytes = m_communicator.recvMessage();

                MessageType messageType = (MessageType)responseBytes[0];

                switch (messageType)
                {
                    case MessageType.JOIN_ROOM:
                        JoinRoomRespnose response = Desrializer.Deserialize<JoinRoomRespnose>(responseBytes);

                        CreateRoomRequest createRoomRequest = new CreateRoomRequest();
                        createRoomRequest.answerTimeout = room.timePerQuestion;
                        createRoomRequest.roomName = room.name;
                        createRoomRequest.questionCount = room.numOfQuestionsInGame;
                        createRoomRequest.maxUsers = room.maxPlayers;

                        handleJoinRoom(response.status, createRoomRequest);
                        break;
                    default:
                        JoinRoom_Click(sender, e);
                        break;
                }
            }
        }

        public void handleJoinRoom(int status, CreateRoomRequest room)
        {
            switch ((ResponseCode)status)
            {
                case ResponseCode.JOIN_ROOM_SUCCESS:
                    
                    NavigationService.Navigate(new InWaitingRoomPage(m_communicator, room));
                    m_isRunning = false;
                    m_updateThread.Join();
                    break;

                case ResponseCode.JOIN_ROOM_FAIL:
                    ErrorMessage.Text = "Join room failed.";
                    break;

                case ResponseCode.ROOM_NOT_FOUND:
                    ErrorMessage.Text = "Room doesnt exist.";
                    break;

                case ResponseCode.USER_ALREADY_IN_ROOM:
                    ErrorMessage.Text = "You are in this room.";
                    break;

                case ResponseCode.ROOM_IS_FULL:
                    ErrorMessage.Text = "Room is full.";
                    break;

                default:
                    break;
            }
        }

        private void ReturnButton_Click(object sender, RoutedEventArgs e)
        {
            NavigationService.Navigate(new MainMenu(m_communicator));
        }
    }
}
