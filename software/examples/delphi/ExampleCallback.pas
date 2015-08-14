program ExampleCallback;

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
    procedure AirPressureCB(sender: TBrickletBarometer; const airPressure: longint);
    procedure AltitudeCB(sender: TBrickletBarometer; const altitude: longint);
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'XYZ'; { Change to your UID }

var
  e: TExample;

{ Callback procedure for air pressure callback (parameter has unit mbar/1000) }
procedure TExample.AirPressureCB(sender: TBrickletBarometer; const airPressure: longint);
begin
  WriteLn(Format('Air Pressure: %f mbar', [airPressure/1000.0]));
end;

{ Callback procedure for altitude callback (parameter has unit cm) }
procedure TExample.AltitudeCB(sender: TBrickletBarometer; const altitude: longint);
begin
  WriteLn(Format('Altitude: %f m', [altitude/100.0]));
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

  { Set period for air pressure callback to 1s (1000ms)
    Note: The air pressure callback is only called every second
          if the air pressure has changed since the last call! }
  b.SetAirPressureCallbackPeriod(1000);

  { Register air pressure callback to procedure AirPressureCB }
  b.OnAirPressure := {$ifdef FPC}@{$endif}AirPressureCB;

  { Set period for altitude callback to 1s (1000ms)
    Note: The altitude callback is only called every second
          if the altitude has changed since the last call! }
  b.SetAltitudeCallbackPeriod(1000);

  { Register altitude callback to procedure AltitudeCB }
  b.OnAltitude := {$ifdef FPC}@{$endif}AltitudeCB;

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy; { Calls ipcon.Disconnect internally }
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
