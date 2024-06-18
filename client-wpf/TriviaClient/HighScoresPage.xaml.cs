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

using TriviaClient;
using static System.Formats.Asn1.AsnWriter;

namespace TriviaClient
{


    public partial class HighScoresPage : Page
    {
        private Communicator m_communicator;

        public HighScoresPage(Communicator communicator)
        {
            InitializeComponent();

            m_communicator = communicator;
            DisplayHighScores();
        }

        private void DisplayHighScores()
        {
            List<HighScores> highScores = topThreeScores();

            if (highScores != null)
            {
                score1.Text = $"{highScores[0].username}: {highScores[0].score}";
                score2.Text = $"{highScores[1].username}: {highScores[1].score}";
                score3.Text = $"{highScores[2].username}: {highScores[2].score}";
            }
            else
            {
                // Handle case when there are not enough high scores available
                MessageBox.Show("Not enough high scores available.");
            }
        }

        private List<HighScores> topThreeScores()
        {
            GetHighScoresRequest highScoresRequest = new GetHighScoresRequest();

            byte[] message = Serializer.Serialize<GetHighScoresRequest>(MessageType.HIGH_SCORE, highScoresRequest);

            m_communicator.sendMessage(message);

            byte[] responseBytes = m_communicator.recvMessage();

            MessageType messageType = (MessageType)responseBytes[0];

            switch (messageType)
            {
                case MessageType.HIGH_SCORE:
                    GetHighScoresResponse response = Desrializer.Deserialize<GetHighScoresResponse>(responseBytes);
                    return response.statistics;
                default:
                    return null;
            }
        }

        private void ReturnButton_Click(object sender, RoutedEventArgs e)
        {
            NavigationService?.Navigate(new GenerakStatisticsPage(m_communicator));
        }
    }
}
