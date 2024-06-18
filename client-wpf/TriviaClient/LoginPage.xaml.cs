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
    /// Interaction logic for LoginPage.xaml
    /// </summary>
    public partial class LoginPage : Page
    {
        enum ResponseCode : int
        {
            LOGIN_SUCCES = 11,
            LOGIN_FAIL = 12,
            USER_DOESNT_EXIST = 13,
            PASSWORD_DOESNT_MATCH = 14,
            USER_ALREADY_LOGGED_IN = 15,
        }


        private Communicator m_communicator;


        public LoginPage(Communicator communicator)
        {
            InitializeComponent();

            m_communicator = communicator;
        }

        private void btnLogin_Click(object sender, RoutedEventArgs e)
        {
            LoginRequest loginRequest = new LoginRequest();
            loginRequest.username = txtUser.Text;
            loginRequest.password = txtPass.Password;

            byte[] message = Serializer.Serialize<LoginRequest>(MessageType.LOG_IN, loginRequest);

            m_communicator.sendMessage(message);

            byte[] responseBytes = m_communicator.recvMessage();

            MessageType messageType = (MessageType)responseBytes[0];

            switch (messageType)
            {
                case MessageType.LOG_IN:
                    LoginRespnose response = Desrializer.Deserialize<LoginRespnose>(responseBytes);
                    handleLogin(response.status);
                    break;
                default:
                    break;
            }
        }
        private void btnSignup_Click(object sender, RoutedEventArgs e)
        {
            NavigationService?.Navigate(new SignUpPage(m_communicator));
        }

        private void handleLogin(int status)
        {
            switch ((ResponseCode)status)
            {
                case ResponseCode.LOGIN_SUCCES:
                    NavigationService?.Navigate(new MainMenu(m_communicator));
                    break;

                case ResponseCode.LOGIN_FAIL:
                    ErrorMessage.Text = "login failed. please check your username and password.";
                    break;

                case ResponseCode.USER_DOESNT_EXIST:
                    ErrorMessage.Text = "user does not exist.";
                    break;

                case ResponseCode.PASSWORD_DOESNT_MATCH:
                    ErrorMessage.Text = "incorrect password.";
                    break;

                case ResponseCode.USER_ALREADY_LOGGED_IN:
                    ErrorMessage.Text = "user is already logged in.";
                    break;

                default:
                    break;
            }
        }
    }
}
