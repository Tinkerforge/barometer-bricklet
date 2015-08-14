program ExampleThreshold;

{$ifdef MSWINDOWS}{$apptype CONSOLE}{$endif}
{$ifdef FPC}{$mode OBJFPC}{$H+}{$endif}

uses
  SysUtils, IPConnection, BrickletBarometer;

type
  TExample = class
  private
    ipcon: TIPConnection;
    b: TBrickletBarometer;
  public
    procedure AirPressureReachedCB(sender: TBrickletBarometer; const airPressure: longint);
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'XYZ'; { Change to your UID }

var
  e: TExample;

{ Callback procedure for air pressure greater than 1025 mbar (parameter has unit mbar/1000) }
procedure TExample.AirPressureReachedCB(sender: TBrickletBarometer; const airPressure: longint);
begin
  WriteLn(Format('Air Pressure: %f mbar', [airPressure/1000.0]));
  WriteLn('Enjoy the potentially good weather!');
end;

procedure TExample.Execute;
begin
  { Create IP connection }
  ipcon := TIPConnection.Create;

  { Create device object }
  b := TBrickletBarometer.Create(UID, ipcon);

  { Connect to brickd }
  ipcon.Connect(HOST, PORT);
  { Don't use device before ipcon is connected }

  { Get threshold callbacks with a debounce time of 10 seconds (10000ms) }
  b.SetDebouncePeriod(10000);

  { Register threshold reached callback to procedure AirPressureReachedCB }
  b.OnAirPressureReached := {$ifdef FPC}@{$endif}AirPressureReachedCB;

  { Configure threshold for "greater than 1025 mbar" (unit is mbar/1000) }
  b.SetAirPressureCallbackThreshold('>', 1025*1000, 0);

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy; { Calls ipcon.Disconnect internally }
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
