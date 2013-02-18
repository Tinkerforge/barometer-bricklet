Imports Tinkerforge

Module ExampleThreshold
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "SCB31" ' Change to your UID

    ' Callback for air pressure greater than 1025 mbar
    Sub ReachedCB(ByVal sender As BrickletBarometer, ByVal airPressure As Integer)
        System.Console.WriteLine("We have " + (airPressure/1000.0).ToString() + " mbar.")
        System.Console.WriteLine("Enjoy the potentially good weather!")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim b As New BrickletBarometer(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get threshold callbacks with a debounce time of 10 seconds (10000ms)
        b.SetDebouncePeriod(10000)

        ' Register threshold reached callback to function ReachedCB
        AddHandler b.AirPressureReached, AddressOf ReachedCB

        ' Configure threshold for "greater than 1025 mbar" (unit is mbar/1000)
        b.SetAirPressureCallbackThreshold(">"C, 1025*1000, 0)

        System.Console.WriteLine("Press key to exit")
        System.Console.ReadKey()
        ipcon.Disconnect()
    End Sub
End Module
