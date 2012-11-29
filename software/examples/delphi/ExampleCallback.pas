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
    procedure AirPressureCB(sender: TObject; const airPressure: longint);
    procedure AltitudeCB(sender: TObject; const altitude: longint);
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'bAc'; { Change to your UID }

var
  e: TExample;

{ Callback function for air pressure callback (parameter has unit mbar/1000) }
procedure TExample.AirPressureCB(sender: TObject; const airPressure: longint);
begin
  WriteLn(Format('AirPressure: %f mbar', [airPressure/1000.0]));
end;

{ Callback function for altitude callback (parameter has unit cm) }
procedure TExample.AltitudeCB(sender: TObject; const altitude: longint);
begin
  WriteLn(Format('Altitude: %f m', [altitude/100.0]));
end;

procedure TExample.Execute;
begin
  { Create IP connection }
  ipcon := TIPConnection.Create();

  { Create device object }
  b := TBrickletBarometer.Create(UID, ipcon);

  { Connect to brickd }
  ipcon.Connect(HOST, PORT);
  { Don't use device before ipcon is connected }

  { Set Period for air pressure and altitude callbacks to 1s (1000ms)
    Note: The air pressure and altitude callbacks are only called every second
          if the air pressure or altitude has changed since the last call! }
  b.SetAirPressureCallbackPeriod(1000);
  b.SetAltitudeCallbackPeriod(1000);

  { Register air pressure callback to procedure AirPressureCB }
  b.OnAirPressure := {$ifdef FPC}@{$endif}AirPressureCB;

  { Register altitude callback to procedure AltitudeCB }
  b.OnAltitude := {$ifdef FPC}@{$endif}AltitudeCB;

  WriteLn('Press key to exit');
  ReadLn;
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
