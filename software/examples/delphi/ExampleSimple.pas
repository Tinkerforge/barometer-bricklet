program ExampleSimple;

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
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'XYZ'; { Change to your UID }

var
  e: TExample;

procedure TExample.Execute;
var airPressure: longint; altitude: longint;
begin
  { Create IP connection }
  ipcon := TIPConnection.Create;

  { Create device object }
  b := TBrickletBarometer.Create(UID, ipcon);

  { Connect to brickd }
  ipcon.Connect(HOST, PORT);
  { Don't use device before ipcon is connected }

  { Get current air pressure (unit is mbar/1000) }
  airPressure := b.GetAirPressure;
  WriteLn(Format('Air Pressure: %f mbar', [airPressure/1000.0]));

  { Get current altitude (unit is cm) }
  altitude := b.GetAltitude;
  WriteLn(Format('Altitude: %f m', [altitude/100.0]));

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy; { Calls ipcon.Disconnect internally }
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
