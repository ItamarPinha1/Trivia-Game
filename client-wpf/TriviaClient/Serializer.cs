using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using TriviaClient;
using System.Text.Json;

namespace TriviaClient
{
    public class Serializer
    {
        public Serializer()
        {
        }
        public static byte[] Serialize<T>(MessageType id, T data)
        {
            string jsonString = JsonSerializer.Serialize(data);
            byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonString);

            int totalLength = 1 + sizeof(int) + jsonBytes.Length;

            byte[] messageBytes = new byte[totalLength];

            messageBytes[0] = (byte)id;

            byte[] lengthBytes = BitConverter.GetBytes(jsonBytes.Length);
            Array.Copy(lengthBytes, 0, messageBytes, 1, lengthBytes.Length);
            Array.Copy(jsonBytes, 0, messageBytes, 1 + lengthBytes.Length, jsonBytes.Length);

            return messageBytes;
        }

    }
}

