// Main.dart 
import 'package:flutter/material.dart';
import 'home_screen.dart'; // Import home screen
void main() {
  runApp(MyApp());
}
class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      title: "MCB Trip Monitoring",
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: HomeScreen(), // Set HomeScreen as the main screen
    );
  }
}

