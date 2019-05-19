import copy
import json
import sys

def get_teams():
    teams = []
    with open("data/teams.csv", "r") as fd:
        for line in fd:
            team = []
            sline = line[:-1].split("\t")
            for e in sline:
                if len(e) > 0:
                    team.append(int(e))
                
            teams.append(team)
    return teams

def find_tour(tours, id):
    for tour in tours:
        if id in tour:
            return tour
    assert False

def check_team_coverage(tours, team):
    tour = find_tour(tours, team[0])
    for tmid in team:
        #print("Check if [%s] is in [%s] " % (tmid, tour))
        if not tmid in tour:
            return False
            #print("Team member [%s] not in tour [%s]" % (tmid, tour))
    #print("Complete team [%s] is in tour [%s]" %(team, tour))
    return True

def check_teams_coverage(tours, teams):
    incomplete_cnts = []
    for team_idx in range(len(teams)):
        if not check_team_coverage(tours, teams[team_idx]):
            print("\tTeam %s is not complete in one tour" % (teams[team_idx]))
            incomplete_cnts.append(1)
        else:
            incomplete_cnts.append(0)

    if incomplete_cnts == (0, 0, 0, 0, 0, 0):
        print("COMPLETE TEAM COVERTAGE", tours)
    return incomplete_cnts

def extract_tours(j):
    tours = []
    for intour in j['tours']:
        tours.append(intour['ids'])
    return tours

def main():
    teams = get_teams()
    team_results = {}

    with open(sys.argv[1], "r") as fd:
        for line in fd:
            j = json.loads(line)
            if j['comment'] != 'final1':
                rating = j['rating']['length-of-stay']
                print("Tour rating [%d]" % (rating))
                tours = extract_tours(j)
                incomplete_cnts = tuple(check_teams_coverage(tours, teams))

                if incomplete_cnts not in team_results:
                    team_results[incomplete_cnts] = (rating, j)
                else:
                    if team_results[incomplete_cnts][0] > rating:
                        team_results[incomplete_cnts] = (rating, j)

    print("Best with complete coverage:")
    print(team_results)

if __name__ == '__main__':
    main()
