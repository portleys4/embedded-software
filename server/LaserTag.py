from AWS import AWS
from Player import Player
from Team import Team
import json
import time

aws_client = AWS("server", 
"a213qaaw8pshxz.iot.us-east-1.amazonaws.com", 
"/home/steve/Desktop/AWS_Certs/RootCA",
"/home/steve/Desktop/AWS_Certs/fc21dd6d85-private.pem.key",
"/home/steve/Desktop/AWS_Certs/fc21dd6d85-certificate.pem.crt")

players = {}
team1 = Team("red", {})
team2 = Team("blue", {})

player_id_counter = 1


def register_player(client, userdata, message):
    global player_id_counter
    player_name = json.loads(message.payload)["player_name"].encode("ascii")
    preferred_team = json.loads(message.payload)["preferred_team"]
    this_player = Player(player_name, player_id_counter)
    player_id_counter += 1
    print(player_name +  " has joined the game")
    players[player_name] = this_player
    player_team = json.loads(message.payload)["player_name"]

def unregister_player(client, userdata, message):
    player_name = json.loads(message.payload)["player_name"]
    del players[player_name]
    print(player_name + " has left the game")

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

#this currently publishes with the victim in the topic
#we need to look up the shooter name with the shoot ID
#and look up the victim ID using the victim name
#then publish to $aws/things/server/" + shooterName + "/gottem", {"victim_ID" : victimID}
def player_hit(client, userdata, message):
    for player in players.keys():
        if player in message.topic:
            players[player].damage(25)
            aws_client.AWS_Publish("$aws/things/server/" + player + "/gottem", {"shooter_ID" : message.payload})

aws_client.AWS_Subscribe("$aws/things/server/lasertag/register", register_player)
aws_client.AWS_Subscribe("$aws/things/server/lasertag/unregister", unregister_player)

print("Starting the game in 30 seconds")
time.sleep(30)

"""
Subscribe to topics for all players in game
Using wildcards doesn't seem to work?
"""
for player_name in players.keys():
    aws_client.AWS_Subscribe("$aws/things/server/lasertag/" + player_name + "/shoot", player_shoot)
    aws_client.AWS_Subscribe("$aws/things/server/lasertag/" + player_name + "/hit", player_hit)

playing_game = True

print("Starting game!")
while playing_game:
    time.sleep(5)
