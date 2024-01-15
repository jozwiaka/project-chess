import chess
import chess.pgn
import os


class PGNReader:
    def __init__(self, pgn_dir):
        self.pgn_dir = pgn_dir

    def read(self):
        games = []
        for filename in os.listdir(self.pgn_dir):
            if filename.endswith(".pgn"):
                pgn_file_path = os.path.join(self.pgn_dir, filename)
                with open(pgn_file_path) as pgn_file:
                    game = chess.pgn.read_game(pgn_file)
                    while game is not None:
                        board = chess.Board()
                        boards = []
                        for move in game.mainline_moves():
                            board.push(move)
                            boards.append(board)
                        games.append(boards)
                        game = chess.pgn.read_game(pgn_file)
        return games


pgn_dir = "../data"
pgn_reader = PGNReader(pgn_dir)
games = pgn_reader.read()
print(games[0])
