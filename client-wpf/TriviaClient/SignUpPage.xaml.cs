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
    /// Interaction logic for SignUpPage.xaml
    /// </summary>
    public partial class SignUpPage : Page
    {

        enum ResponseCode : int
        {
            SIGN_UP_SUCCES = 21,
            SIGN_UP_FAIL = 22,
            USER_EXIST = 23,
        }


        private Communicator m_communicator;
        public SignUpPage(Communicator communicator)
        {
            InitializeComponent();
            m_communicator = communicator;
        }

        private void btnSignUp_Click(object sender, RoutedEventArgs e)
        {


            SignupRequest signupRequest = new SignupRequest();
            signupRequest.username = txtUser.Text;
            signupRequest.password = txtPass.Password;
            signupRequest.email = email.Text;


            byte[] message = Serializer.Serialize<SignupRequest>(MessageType.SIGN_UP, signupRequest);

            m_communicator.sendMessage(message);

            byte[] responseBytes = m_communicator.recvMessage();

            MessageType messageType = (MessageType)responseBytes[0];

            switch (messageType)
            {
                case MessageType.SIGN_UP:
                    SignupRespnose response = Desrializer.Deserialize<SignupRespnose>(responseBytes);
                    handleSignup(response.status);
                    break;
                default:
                    break;
            }
        }

        public void handleSignup(int status)
        {
            switch ((ResponseCode)status)
            {
                case ResponseCode.SIGN_UP_SUCCES:
                    NavigationService?.Navigate(new MainMenu(m_communicator));
                    break;

                case ResponseCode.SIGN_UP_FAIL:
                    ErrorMessage.Text = "SignUp failed.";
                    break;

                case ResponseCode.USER_EXIST:
                    ErrorMessage.Text = "User already exist.";
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
