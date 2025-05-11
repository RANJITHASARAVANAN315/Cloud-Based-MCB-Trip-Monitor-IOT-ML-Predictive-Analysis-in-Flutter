//api_service.dart
import 'package:http/http.dart' as http;
import 'dart:convert';
class AdafruitAPI {
  // Replace with your Adafruit IO Username and Key
  static const String username = "User_name";
  static const String aioKey = "aio_key";
  static const String feedName = "msg";  // Example: "mcb_trip"

  static Future<List<dynamic>> fetchData() async {
    var url = Uri.parse("https://io.adafruit.com/api/v2/$username/feeds/$feedName/data");

    try {
      var response = await http.get(
        url,
        headers: {"X-AIO-Key": aioKey}, // Adafruit IO authentication
      );

      if (response.statusCode == 200) {
        return jsonDecode(response.body); // Return the JSON response as a list
      } else {
        throw Exception("Failed to fetch data. Status: ${response.statusCode}");
      }
    } catch (e) {
      throw Exception("Error fetching data: $e");
    }
  }
}
