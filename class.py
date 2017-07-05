class Personne:
    def __init__(self,prenom,nom):
        self.prenom=prenom
        self.nom=nom
        self.ville="inconnu"
        self.age=0
    def demenager(self,ville):
        print(self.nom,self.prenom,"demenage vers",ville)
        self.ville=ville

p=Personne("luk","skywalker")
