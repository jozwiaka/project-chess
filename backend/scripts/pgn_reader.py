import chess
import chess.pgn


class PGNReader:
    def __init__(self, pgn_file_path):
        self.pgn_file_path = pgn_file_path

    def read(self):
        games = []
        with open(self.pgn_file_path) as pgn_file:
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


pgn_file_path = "../data/London2g6.pgn"
pgn_viewer = PGNReader(pgn_file_path)
games = pgn_viewer.read()
print(games[0])
