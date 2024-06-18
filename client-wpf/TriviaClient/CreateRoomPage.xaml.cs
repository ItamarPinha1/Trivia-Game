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

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for CreateRoomPage.xaml
    /// </summary>
    public partial class CreateRoomPage : Page
    {
        enum ResponseCode : int
        {
            CREATE_ROOM_SUCCESS = 41,
            CREATE_ROOM_FAIL = 42,
            ROOM_ALREADY_EXISTS = 44,
        }

        private Communicator m_communicator;

        public CreateRoomPage(Communicator communicator)
        {
            InitializeComponent();
            m_communicator = communicator;
        }

        private void CreateRoomButton_Click(object sender, RoutedEventArgs e)
        {

            CreateRoomRequest createRoomRequest = new CreateRoomRequest();
            createRoomRequest.roomName = RoomNameTextBox.Text;
            createRoomRequest.maxUsers = (int)NumPlayersSlider.Value;

            //createRoomRequest.questionCount = (int)NumQuestionsSlider.Value;
            createRoomRequest.questionCount = 3;

            createRoomRequest.answerTimeout = (int)QuestionTimeSlider.Value;

            byte[] message = Serializer.Serialize<CreateRoomRequest>(MessageType.CREATE_ROOM, createRoomRequest);

            m_communicator.sendMessage(message);

            byte[] responseBytes = m_communicator.recvMessage();

            MessageType messageType = (MessageType)responseBytes[0];

            switch (messageType)
            {
                case MessageType.CREATE_ROOM:
                    CreateRoomRespnose response = Desrializer.Deserialize<CreateRoomRespnose>(responseBytes);
                    handleCreateRoom(response.status, createRoomRequest);
                    break;
                default:
                    break;
            }
        }

        private void handleCreateRoom(int status, CreateRoomRequest createRoomRequest)
        {
            switch ((ResponseCode)status)
            {
                case ResponseCode.CREATE_ROOM_SUCCESS:
                    NavigationService.Navigate(new inWaitingRoomAdminPage(m_communicator, createRoomRequest));
                    break;

                case ResponseCode.CREATE_ROOM_FAIL:
                    ErrorMessage.Text = "Create room failed.";
                    break;

                case ResponseCode.ROOM_ALREADY_EXISTS:
                    ErrorMessage.Text = "Room already exist.";
                    break;

                default:
                    break;
            }
        }


        private void ReturnButton_Click(object sender, RoutedEventArgs e)
        {
            NavigationService?.Navigate(new MainMenu(m_communicator));
        }

    }
}
