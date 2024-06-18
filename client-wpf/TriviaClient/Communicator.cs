using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;




namespace TriviaClient
{

    public enum MessageType : byte
    {
        LOG_IN = 10,
        SIGN_UP = 20,
        LOG_OUT = 30,
        CREATE_ROOM = 40,
        GET_ROOMS = 50,
        GET_PLAYERS_IN_ROOM = 60,
        JOIN_ROOM = 70,
        GET_STATISTICS = 80,
        HIGH_SCORE = 90,
        CLOSE_ROOM = 100,
        START_GAME = 110,
        GET_ROOM_STATE = 120,
        LEAVE_ROOM = 130,
        LEAVE_GAME = 140,
        GET_QUESTION = 150,
        SUBMIT_ANSWER = 160,
        GET_GAME_RESULTS = 170

    }

    public class Communicator
    {
        private NetworkStream m_socket;
        private int m_port;
        private const int BufferSize = 4096;

        private Mutex mtx;

        public Communicator(int port)
        {
            m_port = port;

            mtx = new Mutex();
        }

        public bool connectToServer()
        {
            try
            {
                TcpClient client = new TcpClient();

                IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), m_port);

                client.Connect(serverEndPoint);

                m_socket = client.GetStream();

                return true;
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error connecting to server: " + ex.Message);
            }

            return false;
        }

        public void sendMessage(byte[] data)
        {
            try
            {
                mtx.WaitOne();

                m_socket.Write(data, 0, data.Length);
                m_socket.Flush();
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error sending message to server: " + ex.Message);
                mtx.ReleaseMutex();
            }

            mtx.ReleaseMutex();
        }

        public byte[] recvMessage()
        {
            try
            {
                mtx.WaitOne();

                byte[] codeBuffer = new byte[1];
                m_socket.Read(codeBuffer, 0, 1);

                byte[] bufferLen = new byte[4];
                m_socket.Read(bufferLen, 0, 4);

                int len = BitConverter.ToInt32(bufferLen, 0);

                byte[] buffer = new byte[len];
                m_socket.Read(buffer, 0, len);

                byte[] fullMessage = new byte[1 + 4 + len];
                Buffer.BlockCopy(codeBuffer, 0, fullMessage, 0, 1);
                Buffer.BlockCopy(bufferLen, 0, fullMessage, 1, 4);
                Buffer.BlockCopy(buffer, 0, fullMessage, 5, len);

                mtx.ReleaseMutex();
                return fullMessage;
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error getting response from server: " + ex.Message);

                mtx.ReleaseMutex();
                return null;
            }
        }

        private string covertBytesToString(byte[] buffer, int numberOfBytes)
        {
            return Encoding.ASCII.GetString(buffer, 0, numberOfBytes);
        }

    }
}