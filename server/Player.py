class Player:
    def __init__(self, name, player_id, team):
        self.name = name
        self.player_id = player_id
        self.shots = 0
        self.health = 100
        self.lives = 3
        self.score = 0
        self.team = team

    def add_score(self, points):
        self.score += points

    def damage(self, damage):
        if damage > self.health:
            if self.lives <= 1:
                self.lives = 0#Dead
            else:
                self.lives -= 1
                self.health = 100
        else:
            self.health -= damage

    def add_shots(self, shots):
        self.shots += shots