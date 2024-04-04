package main

import (
	"fmt"
	"math/rand"
	"os"

	"github.com/charmbracelet/bubbletea"
)

type Bedtjed struct {
	insults       []string
	currentInsult int
}

func makeState() Bedtjed {
	return Bedtjed{
		insults: []string{
			"piss off",
			"ya 3abit",
			"ya hmar",
			"you look like a donut",
			"unholy awk scripters like you should piss off!",
			"youre fat",
			"youre obese",
			"retarded duck",
			"imagine writing a quran scraper in *shell* to \"repent\"",
			"why did you get so close with a fucking 30 year old 10,000km away?",
			"your dick is small",
		},
		currentInsult: 0,
	}
}

func (b Bedtjed) Init() tea.Cmd {
	return nil
}

func (b Bedtjed) Update(msg tea.Msg) (tea.Model, tea.Cmd) {
	switch msg := msg.(type) {
	case tea.KeyMsg:
		switch msg.String() {
		case "ctrl+c", "q", "esc":
			return b, tea.Quit
		default:
			b.currentInsult = rand.Int() % len(b.insults)
		}
	}

	return b, nil
}

func (b Bedtjed) View() string {
	return fmt.Sprintf("your current insult: %s :D (press any key)", b.insults[b.currentInsult])
}

func main() {
	program := tea.NewProgram(makeState())
	if _, err := program.Run(); err != nil {
		fmt.Printf("ERROR ERROR ERROR ABORT GET OUT OF THIS PLACE OwO")
		os.Exit(69)
	}
}
