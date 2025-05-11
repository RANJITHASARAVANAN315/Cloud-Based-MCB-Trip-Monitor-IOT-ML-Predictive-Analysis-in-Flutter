// Home_screen.dart 
import 'package:flutter/material.dart';
import 'services/api_service.dart'; // Import Adafruit API service

class HomeScreen extends StatefulWidget {
  @override
  _HomeScreenState createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> {
  List<dynamic> adafruitData = [];

  @override
  void initState() {
    super.initState();
    loadData();
  }

  // Fetch data from Adafruit IO API
  void loadData() async {
    try {
      List<dynamic> data = await AdafruitAPI.fetchData();
      setState(() {
        adafruitData = data;
      });
    } catch (e) {
      print("Error fetching data: $e");
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text("MCB Trip Monitoring"),
        backgroundColor: Colors.blueAccent, // Keep blue UI
      ),
      body: adafruitData.isEmpty
          ? Center(child: CircularProgressIndicator()) // Show loading
          : ListView.builder(
        itemCount: adafruitData.length,
        itemBuilder: (context, index) {
          return Card(
            margin: EdgeInsets.symmetric(vertical: 8, horizontal: 16),
            child: ListTile(
              leading: Icon(Icons.electric_bolt, color: Colors.redAccent),
              title: Text(
                "Value: ${adafruitData[index]['value']}",
                style: TextStyle(fontWeight: FontWeight.bold),
              ),
              subtitle: Text("Time: ${adafruitData[index]['created_at']}"),
            ),
          );
        },
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: loadData, // Refresh data manually
        child: Icon(Icons.refresh),
        backgroundColor: Colors.blueAccent,
      ),
    );
  }
}
