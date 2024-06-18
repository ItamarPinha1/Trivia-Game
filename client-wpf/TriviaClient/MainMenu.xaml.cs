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
    /// Interaction logic for MainMenu.xaml
    /// </summary>
    public partial class MainMenu : Page
    {

        enum ResponseCode : int
        {
            LOGOUT_SUCCES = 31,
            LOGGOUT_FAIL = 32,
            USER_ISNT_LOGGED_IN = 33,

            CREATE_ROOM_SUCCESS = 41,
            CREATE_ROOM_FAIL = 42,
            ROOM_NOT_FOUND = 43,
            ROOM_ALREADY_EXISTS = 44,
            ROOM_DELETED_SUCCESSFULLY = 45,
            USER_ALREADY_IN_ROOM = 46,
            ROOM_IS_FULL = 47,

        }


        private Communicator m_communicator;
        public MainMenu(Communicator communicator)
        {
            InitializeComponent();
            m_communicator = communicator;
        }

        private void SignOutButton_Click(object sender, RoutedEventArgs e)
        {

            SignOutRequest signOutRequest  = new SignOutRequest();

            byte[] message = Serializer.Serialize<SignOutRequest>(MessageType.LOG_OUT, signOutRequest);

            m_communicator.sendMessage(message);

            byte[] responseBytes = m_communicator.recvMessage();

            MessageType messageType = (MessageType)responseBytes[0];

            switch (messageType)
            {
                case MessageType.LOG_OUT:
                    SignOutRespnose response = Desrializer.Deserialize<SignOutRespnose>(responseBytes);
                    handleLogOut(response.status);
                    break;
                default:
                    break;
            }
        }

        private void handleLogOut(int status)
        {
            switch ((ResponseCode)status)
            {
                case ResponseCode.LOGOUT_SUCCES:
                    NavigationService?.Navigate(new LoginPage(m_communicator));
                    break;

                case ResponseCode.LOGGOUT_FAIL:
                    ErrorMessage.Text = "Logout failed.";
                    break;

                case ResponseCode.USER_ISNT_LOGGED_IN:
                    ErrorMessage.Text = "User isn't logged in.";
                    break;

                default:
                    break;
            }
        }

        private void JoinRoomButton_Click(object sender, RoutedEventArgs e)
        {
            NavigationService.Navigate(new JoinRoomPage(m_communicator));
        }

        private void CreateRoomButton_Click(object sender, RoutedEventArgs e)
        {
            NavigationService.Navigate(new CreateRoomPage(m_communicator));
        }

        private void QuitButton_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();
        }

        private void StatisticsButton_Click(object sender, RoutedEventArgs e)
        {
            NavigationService?.Navigate(new GenerakStatisticsPage(m_communicator));
        }
    }
}
