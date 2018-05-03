class Team:
    def __init__(self, name):
        self.name = name
        self.players = []
        self.score = 0
        self.playing = True

    def add_player(self, player):
        self.players[player.name] = player

    def remove_player(self, player):
        del self.players[player.name]
        if len(self.players) == 0:
            self.playing = False
