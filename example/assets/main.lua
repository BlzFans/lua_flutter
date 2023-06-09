

local widget = Center {
    child = Text(
        "flutter_main", {
            style = TextStyle {
                fontWeight = FontWeight.bold,
                color = Colors.blue,
            },
            textDirection = TextDirection.ltr,
        }
    )
}

runApp(widget)

