using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using TriviaClient;
using System.Text.Json;

namespace TriviaClient
{
    public class Desrializer
    {
        public Desrializer()
        {
        }

        public static T Deserialize<T>(byte[] messageBytes)
        {
            byte messageTypeByte = messageBytes[0];
            int bufferLength = BitConverter.ToInt32(messageBytes, 1);

            string jsonString = Encoding.UTF8.GetString(messageBytes, sizeof(int) + sizeof(byte), bufferLength);
            return JsonConvert.DeserializeObject<T>(jsonString);
        }

        public static GetQuestionResponse DeserializeGetQuestionResponse(byte[] messageBytes)
        {
            byte messageTypeByte = messageBytes[0];
            int bufferLength = BitConverter.ToInt32(messageBytes, 1);

            string jsonString = Encoding.UTF8.GetString(messageBytes, sizeof(int) + sizeof(byte), bufferLength);

            var jsonObject = JsonConvert.DeserializeObject<Dictionary<string, object>>(jsonString);
            var answersArray = JsonConvert.DeserializeObject<List<List<object>>>(jsonObject["answers"].ToString());

            GetQuestionResponse response = new GetQuestionResponse
            {
                status = Convert.ToInt32(jsonObject["status"]),
                question = jsonObject["question"].ToString(),
                answers = ConvertToDictionary(answersArray)
            };

            return response;
        }

        public static Dictionary<int, string> ConvertToDictionary(List<List<object>> array)
        {
            var dictionary = new Dictionary<int, string>();

            foreach (var item in array)
            {
                int key = Convert.ToInt32(item[0]);
                string value = item[1].ToString();
                dictionary[key] = value;
            }

            return dictionary;
        }


    }
}
