import json
import time
from AWS import AWS
from Player import Players

aws_client = AWS("server", 
"a213qaaw8pshxz.iot.us-east-1.amazonaws.com", 
"/home/steve/Desktop/AWS_Certs/RootCA",
"/home/steve/Desktop/AWS_Certs/fc21dd6d85-private.pem.key",
"/home/steve/Desktop/AWS_Certs/fc21dd6d85-certificate.pem.crt")

Game_Players = Players()


def register_player(client, userdata, message):
    jsonMessage = json.loads(message.payload)
    player_name = jsonMessage["player_name"].encode("ascii")
    if "preferred_team" in jsonMessage:
        preferred_team = jsonMessage["preferred_team"].encode("ascii")
        Game_Players.add_player(player_name, preferred_team)
    else:
        Game_Players.add_player(player_name)
    print(player_name +  " has joined the game on team " + preferred_team)

def unregister_player(client, userdata, message):
    player_name = json.loads(message.payload)["player_name"]
    Game_Players.remove_player(player_name)
    print(player_name + " has left the game")

def player_shoot(client, userdata, message):
    """
    Number of shots player has taken since the last time it was recorded
    """
    player_name = message.topic[28:]
    player_name = player_name[:-6]
    shots_fired = json.loads(message.payload)["shots_fired"]
    shots_fired = int(shots_fired)
    Game_Players.add_shots(player_name, shots_fired)

#this currently publishes with the victim in the topic
#we need to look up the shooter name with the shoot ID
#and look up the victim ID using the victim name
#then publish to $aws/things/server/" + shooterName + "/gottem", {"victim_ID" : victimID}
def player_hit(client, userdata, message):
    jsonMessage = json.loads(message.payload)
    victim_name = message.topic[28:]
    victim_name = victim_name[:-6]
    Game_Players.damage_player(victim_name, 20)
    for potential_shooter in Game_Players.get_players():
        if potential_shooter.player_id is int(jsonMessage["shooter_ID"]):
            victim_id = Game_Players.get_playerid(victim_name)
            topic = "$aws/things/server/" + potential_shooter + "/gottem"
            aws_client.AWS_Publish(topic, {"victim_ID" : victim_id})
            break

aws_client.AWS_Subscribe("$aws/things/server/lasertag/register", register_player)
aws_client.AWS_Subscribe("$aws/things/server/lasertag/unregister", unregister_player)

print("Starting the game in 60 seconds")
time.sleep(60)

"""
Subscribe to topics for all players in game
Using wildcards doesn't seem to work?
"""
for player in Game_Players.get_players():
    aws_client.AWS_Subscribe("$aws/things/server/lasertag/" + player + "/shoot", player_shoot)
    aws_client.AWS_Subscribe("$aws/things/server/lasertag/" + player + "/hit", player_hit)

playing_game = True

print("Starting game!")
while playing_game:
    time.sleep(5)
