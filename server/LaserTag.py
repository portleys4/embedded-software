import json
import time
from AWS import AWS
from Player import Players

SHOT_DAMAGE = 20
LIVES = 3
RESPAWN = 30
FIRE_RATE = 1000
MODE = "last_team_standing"
POINTS_PER_HIT = 1
POINTS_PER_KILL = 10

RULES = {
    "shot_damage" : SHOT_DAMAGE,
    "lives" : LIVES,
    "respawn" : 30,
    "fire_rate" : FIRE_RATE,
    "mode" : MODE,
    "points_per_hit" : POINTS_PER_HIT,
    "points_per_kill" : POINTS_PER_KILL
}

aws_client = AWS("server", 
"a213qaaw8pshxz.iot.us-east-1.amazonaws.com", 
"/home/steve/Desktop/AWS_Certs/RootCA",
"/home/steve/Desktop/AWS_Certs/fc21dd6d85-private.pem.key",
"/home/steve/Desktop/AWS_Certs/fc21dd6d85-certificate.pem.crt")

Game_Players = Players()

def json_clean(jsonMessage):
    if jsonMessage.endswith('\x00'):
        return jsonMessage[:-1]
    else:
        return jsonMessage

def register_player(client, userdata, message):
    message.payload = json_clean(message.payload)
    jsonMessage = json.loads(message.payload)
    player_name = jsonMessage["player_name"].encode("ascii")
    if "preferred_team" in jsonMessage:
        preferred_team = jsonMessage["preferred_team"].encode("ascii")
        Game_Players.add_player(player_name, preferred_team)
        print(player_name +  " has joined the game on team " + preferred_team)
    else:
        Game_Players.add_player(player_name)
        print(player_name +  " has joined the game on team 'Default'")

def unregister_player(client, userdata, message):
    message.payload = json_clean(message.payload)
    player_name = json.loads(message.payload)["player_name"].encode("ascii")
    Game_Players.remove_player(player_name)
    print(player_name + " has left the game")

def player_shoot(client, userdata, message):
    """
    Number of shots player has taken since the last time it was recorded
    """
    message.payload = json_clean(message.payload)
    player_name = message.topic.encode("ascii")
    player_name = player_name[28:]
    player_name = player_name[:-6]
    shots_fired = json.loads(message.payload)["shots_fired"]
    shots_fired = int(shots_fired)
    Game_Players.add_shots(player_name, shots_fired)

#Need to check and make sure players are on opposite teams
#Give shooter score
def player_hit(client, userdata, message):
    message.payload = json_clean(message.payload)
    jsonMessage = json.loads(message.payload)
    victim_name = message.topic.encode("ascii")
    victim_name = victim_name[28:]
    victim_name = victim_name[:-6]
    shooter_id = int(jsonMessage["id"])
    shooter_name = Game_Players.player_hit(victim_name, shooter_id, 20)
    victim_id = Game_Players.get_playerid(victim_name)
    if shooter_name:
        topic = "$aws/things/server/" + shooter_name + "/gottem"
        aws_client.AWS_Publish(topic, {"victim_ID" : victim_id})
        print shooter_name + " has hit " + victim_name
    else:
        print "Friendly fire between" + shooter_name + " and " + victim_name

aws_client.AWS_Subscribe("$aws/things/server/lasertag/register", register_player)
aws_client.AWS_Subscribe("$aws/things/server/lasertag/unregister", unregister_player)

raw_input("Type something to start the game!")

aws_client.AWS_Unsubscribe("$aws/things/server/lasertag/register")
aws_client.AWS_Unsubscribe("$aws/things/server/lasertag/unregister")

print("Starting the game in 15 seconds")
time.sleep(15)


"""
Subscribe to topics for all players in game
Using wildcards doesn't seem to work?
"""
for player in Game_Players.get_players():
    aws_client.AWS_Subscribe("$aws/things/" + player.get_name() + "lasertag/shoot", player_shoot)
    aws_client.AWS_Subscribe("$aws/things/" + player.get_name() + "lasertag/hit", player_hit)


print("Starting game!")
aws_start_topic = "$aws/things/server/lasertag/start"
start_message = RULES #Fix this
print "Publishing to " + aws_start_topic " to begin the game"
aws_client.AWS_Publish(aws_start_topic, start_message)
while True:
    time.sleep(5)
