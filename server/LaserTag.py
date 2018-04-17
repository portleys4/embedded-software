from AWS import AWS
from Player import Player
import json
import time

aws_client = AWS("server", 
"a213qaaw8pshxz.iot.us-east-1.amazonaws.com", 
"/home/steven/Desktop/AWS_Certs/RootCA",
"/home/steven/Desktop/AWS_Certs/fc21dd6d85-private.pem.key",
"/home/steven/Desktop/AWS_Certs/fc21dd6d85-certificate.pem.crt")

players = {}
player_id_counter = 1
playing_game = False


def register_player(client, userdata, message):
    global player_id_counter
    player_name = json.loads(message.payload)["player_name"]
    this_player = Player(player_name, player_id_counter)
    player_id_counter += 1
    players[player_name] = this_player

def unregister_player(client, userdata, message):
    player_name = json.loads(message.payload)["player_name"]
    del players[player_name]

def player_shoot(client, userdata, message):
    """
    Number of shots player has taken since the last time it was recorded
    """
    for player in players.keys():
        if player in message.topic:
            shots_fired = json.loads(message.payload)["shots_fired"]
            shots_fired = int(shots_fired)
            players[player].add_shots(shots_fired)
            break

def player_hit(client, userdata, message):
    for player in players.keys():
        if player in message.topic:
            players[player].damage(20)
            aws_client.AWS_Publish("$aws/things/server/" + player + "/gottem", {"shooter_ID" : message.payload})

aws_client.AWS_Subscribe("$aws/things/server/lasertag/register", register_player)
aws_client.AWS_Subscribe("$aws/things/server/lasertag/unregister", unregister_player)

while not playing_game:
    usr_input = raw_input("Type start to start the game once all players are registered:\n")
    print usr_input
    if usr_input == "start":
        print "test"
        playing_game = True


"""
Subscribe to topics for all players in game
Using wildcards doesn't seem to work?
"""
for player_name in players.keys():
    aws_client.AWS_Subscribe("$aws/things/server/lasertag/" + player_name + "/shoot", player_shoot)
    aws_client.AWS_Subscribe("$aws/things/server/lasertag/" + player_name + "/hit", player_hit)

print("Starting game!")
while(playing_game):
    print("Playing game")
