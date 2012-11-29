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
    procedure ReachedCB(sender: TObject; const airPressure: longint);
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'bAc'; { Change to your UID }

var
  e: TExample;

{ Callback for air pressure greater than 1025 mbar }
procedure TExample.ReachedCB(sender: TObject; const airPressure: longint);
begin
  WriteLn(Format('We have %f mbar.', [airPressure/1000.0]));
  WriteLn('Enjoy the potentially good weather!');
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

  { Get threshold callbacks with a debounce time of 10 seconds (10000ms) }
  b.SetDebouncePeriod(10000);

  { Register threshold reached callback to procedure ReachedCB }
  b.OnAirPressureReached := {$ifdef FPC}@{$endif}ReachedCB;

  { Configure threshold for "greater than 1025 mbar" (unit is mbar/1000) }
  b.SetAirPressureCallbackThreshold('>', 1025*1000, 0);

  WriteLn('Press key to exit');
  ReadLn;
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
