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
  UID = 'bAc'; { Change to your UID }

var
  e: TExample;

procedure TExample.Execute;
var airPressure, altitude: longint;
begin
  { Create IP connection to brickd }
  ipcon := TIPConnection.Create(HOST, PORT);

  { Create device object }
  b := TBrickletBarometer.Create(UID);

  { Add device to IP connection }
  ipcon.AddDevice(b);
  { Don't use device before it is added to a connection }

  { Get current air pressure (unit is mbar/1000) }
  airPressure := b.GetAirPressure;
  WriteLn(Format('Air Pressure: %f mbar', [airPressure/1000.0]));

  { Get current altitude (unit is cm) }
  altitude := b.GetAltitude;
  WriteLn(Format('Altitude: %f m', [altitude/100.0]));

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy;
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
