using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;
using static System.Formats.Asn1.AsnWriter;

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for GameScreen.xaml
    /// </summary>
    public partial class GameScreen : Page
    {

        enum ResponseCode : int
        {
            GAME_FINISHED = 153,
        }

        private Communicator m_communicator;
        private Thread m_getQuestionThread;
        private bool m_isGetQuestionRunning;
        int m_timePerQuestion;
        int m_questionNum;
        int m_questionsCount;
        int m_questionsLeft;
        private DispatcherTimer _timer;
        private int _timeCounter;

        int m_correctAnswerId;

        public GameScreen(Communicator communicator, int timePerQuestion, int questionsCount)
        {
            InitializeComponent();
            m_communicator = communicator;
            m_timePerQuestion = timePerQuestion;
            _timeCounter = m_timePerQuestion;
            m_questionNum = 0;
            m_questionsCount= questionsCount;

            m_isGetQuestionRunning = true;
            m_getQuestionThread = new Thread(() => UpdateQuestion());
            m_getQuestionThread.Start();

            InitializeTimer();


        }

        private void InitializeTimer()
        {
            _timer = new DispatcherTimer();
            _timer.Interval = TimeSpan.FromSeconds(1); // Set the timer interval to 1 second
            _timer.Tick += Timer_Tick;
            _timer.Start();
        }
        private void Timer_Tick(object sender, EventArgs e)
        {
            Timer.Text = $"{_timeCounter} ";
            _timeCounter--;
        }

        private void UpdateQuestion()
        {
            while (m_isGetQuestionRunning)
            {
                handleGetQuestion();
                Thread.Sleep(200);
            }
        }

        public void handleGetQuestion()
        {
            GetQuestionRequest questionRequest = new GetQuestionRequest();

            byte[] message = Serializer.Serialize<GetQuestionRequest>(MessageType.GET_QUESTION, questionRequest);

            m_communicator.sendMessage(message);

            byte[] responseBytes = m_communicator.recvMessage();

            MessageType messageType = (MessageType)responseBytes[0];

            switch (messageType)
            {
                case MessageType.GET_QUESTION:
                    GetQuestionResponse response = Desrializer.DeserializeGetQuestionResponse(responseBytes);

                    if(response.status == (int)ResponseCode.GAME_FINISHED)
                    {
                        m_isGetQuestionRunning = false;
                        Application.Current.Dispatcher.Invoke(() => 
                        { 
                            NavigationService?.Navigate(new AfterGameScoreScreenPage(m_communicator));
                        });
                        break;
                    }
                    printQuestion(response);
                    break;
                default:
                    break;
            }
        }

        public void printQuestion(GetQuestionResponse question)
        {
            try
            {
                Application.Current.Dispatcher.Invoke(() =>
                {
                    // Add the question and each answer 
                    DisplayQuestion(question);
                });
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error printing player names: " + ex.Message);
            }
        }

        private void DisplayQuestion(GetQuestionResponse questionDetials)
        {
            Dictionary<int, string> answers = questionDetials.answers;
            string question_str = questionDetials.question;

            if (question_str != null)
            {
                if (answers != null && question_str != question.Text)
                {
                    _timeCounter = m_timePerQuestion;
                    m_questionNum++;
                    m_questionsLeft = m_questionsCount - m_questionNum;

                    ResetAnswerButtonBorders();

                    question.Text = $"{question_str}";
                    questionNum.Text = $"Question Number: {m_questionNum}";
                    questionsLeft.Text = $"Questions Left: {m_questionsLeft}";

                    answer1.Content = $"{answers[0]}";
                    answer2.Content = $"{answers[1]}";
                    answer3.Content = $"{answers[2]}";
                    answer4.Content = $"{answers[3]}";

                    SetAnswerButtonsEnabled(true);
                }
            }
            else
            {
                MessageBox.Show("No question to display");
            }
        }

        private void answer1_Click(object sender, RoutedEventArgs e)
        {
            int correctId = sendAnswer(0);
            
            changeColor(correctId);

            SetAnswerButtonsEnabled(false);


        }

        private void answer2_Click(object sender, RoutedEventArgs e)
        {
            int correctId = sendAnswer(1);

            changeColor(correctId);

            SetAnswerButtonsEnabled(false);


        }

        private void answer3_Click(object sender, RoutedEventArgs e)
        {
            int correctId = sendAnswer(2);

            changeColor(correctId);

            SetAnswerButtonsEnabled(false);


        }

        private void answer4_Click(object sender, RoutedEventArgs e)
        {
            int correctId = sendAnswer(3);

            changeColor(correctId);

            SetAnswerButtonsEnabled(false);


        }


        private void SetAnswerButtonsEnabled(bool isEnabled)
        {
            answer1.IsEnabled = isEnabled;
            answer2.IsEnabled = isEnabled;
            answer3.IsEnabled = isEnabled;
            answer4.IsEnabled = isEnabled;
        }

        private void Page_Unloaded(object sender, RoutedEventArgs e)
        {
            m_isGetQuestionRunning = false;
            m_getQuestionThread.Join();
        }

        public int sendAnswer(int id)
        {
            SubmitAnswerRequest submitAnswerRequest = new SubmitAnswerRequest();
            submitAnswerRequest.answerId = id;

            byte[] message = Serializer.Serialize<SubmitAnswerRequest>(MessageType.SUBMIT_ANSWER, submitAnswerRequest);

            m_communicator.sendMessage(message);
            byte[] responseBytes = m_communicator.recvMessage();

            SubmitAnswerResponse response = Desrializer.Deserialize<SubmitAnswerResponse>(responseBytes);
            return response.correctAnswerId;
            
        }

        public void changeColor(int correctAnswerId)
        {
            SolidColorBrush correctBrush = new SolidColorBrush(Colors.Green);
            SolidColorBrush incorrectBrush = new SolidColorBrush(Colors.Red);

            // Reset all border colors to default before setting the correct answer's color
            ResetAnswerButtonBorders();

            switch (correctAnswerId)
            {
                case 0:
                    answer1.BorderBrush = correctBrush; // Correct answer outline to green
                    answer2.BorderBrush = incorrectBrush; // Incorrect answer outline to red
                    answer3.BorderBrush = incorrectBrush;
                    answer4.BorderBrush = incorrectBrush;
                    break;
                case 1:
                    answer1.BorderBrush = incorrectBrush;
                    answer2.BorderBrush = correctBrush;
                    answer3.BorderBrush = incorrectBrush;
                    answer4.BorderBrush = incorrectBrush;
                    break;
                case 2:
                    answer1.BorderBrush = incorrectBrush;
                    answer2.BorderBrush = incorrectBrush;
                    answer3.BorderBrush = correctBrush;
                    answer4.BorderBrush = incorrectBrush;
                    break;
                case 3:
                    answer1.BorderBrush = incorrectBrush;
                    answer2.BorderBrush = incorrectBrush;
                    answer3.BorderBrush = incorrectBrush;
                    answer4.BorderBrush = correctBrush;
                    break;
                default:
                    Console.WriteLine("Invalid correct answer ID");
                    break;
            }
        }

        public void ResetAnswerButtonBorders()
        {
            SolidColorBrush defaultBorderBrush = new SolidColorBrush(Colors.Black); // Default border color
            answer1.BorderBrush = defaultBorderBrush;
            answer2.BorderBrush = defaultBorderBrush;
            answer3.BorderBrush = defaultBorderBrush;
            answer4.BorderBrush = defaultBorderBrush;
        }


    }
}
