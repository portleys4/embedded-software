import time

class Player(object):
    def __init__(self, name, player_id, team_name):
        self.name = name
        self.player_id = player_id
        self.shots = 0
        self.health = 100
        self.lives = 3
        self.score = 0
        self.team_name = team_name
        self.active = True #Are you currently in play? Turns False when waiting to respawn

    def add_score(self, points):
        self.score += points

    def damage(self, damage):
        if damage >= self.health:
            if self.lives <= 1:
                self.lives = 0#Dead
                return True
            else: #Die but still have at least one more life
                self.lives -= 1
                self.health = 100
                self.active = False
                time.sleep(20)
                self.active = True
        else:
            self.health -= damage
        return False

    def get_name(self):
        return self.name

    def add_shots(self, shots):
        self.shots += shots

    def get_playerid(self):
        return self.player_id

class Players(object):
    def __init__(self):
        self.players = {}
        self.team1 = []
        self.team2 = []
        self.player_counter = 0

    def player_hit(self, victim_name, shooter_id, damage):
        victim = self.players[victim_name]
        for player in self.players.values():
            print("Player ID: " + str(player.player_id))
            print("Shooter ID: " + str(shooter_id))
            if player.player_id == shooter_id:
                shooter = player
                break
        else:
            return -1 
        if victim.team_name == shooter.team_name:
            print("Friendly fire between " + str(victim_name) + " and " + str(shooter.name))
            return 0 #Friendly fire
        if self.damage_player(victim_name, damage):
            self.add_score(shooter.name, 10)
        else:
            self.add_score(shooter.name, 1)
        return shooter.name

    def add_player(self, player_name, preferred_team = None):
        if preferred_team == "team1":
            self.players[player_name] = Player(player_name, self.player_counter, "team1")
            self.team1.append({"name" : player_name, "id" : self.player_counter})
            print(player_name + " has join the game on team " + "team1")
        elif preferred_team == "team2":
            self.team2.append({"name" : player_name, "id" : self.player_counter})
            self.players[player_name] = Player(player_name, self.player_counter, "team2")
            print(player_name + " has join the game on team " + "team2")
        else:
            if len(self.team1) <= len(self.team2):
                self.players[player_name] = Player(player_name, self.player_counter, "team1")
                self.team1.append({"name" : player_name, "id" : self.player_counter})
                print(player_name + " has join the game on team 1")
            else:
                self.players[player_name] = Player(player_name, self.player_counter, "team2")
                self.team2.append({"name" : player_name, "id" : self.player_counter})
                print(player_name + " has join the game on team 2")
        self.player_counter += 1

    def remove_player(self, player_name):
        del self.players[player_name]

    def damage_player(self, player_name, damage):
        print(str(damage) + " damage dealt to " + player_name)
        if self.players[player_name].damage(damage):
            print(player_name + " has been killed")
            del self.players[player_name]
            return True
        return False

    def add_score(self, player_name, score):
        print(str(score) + " added to " + player_name + "'s score")
        self.players[player_name].add_score(score)

    def add_shots(self, player_name, shots):
        print(shots + " added to " + player_name + "'s shots")
        self.players[player_name].add_shots(shots)

    def get_players(self):
        return self.players.values()

    def get_playerid(self, player_name):
        return self.players[player_name].get_playerid()
