import { useState } from 'react';
import Header from './components/Header';
import SeasonView from './components/SeasonView';
import ScheduleView from './components/ScheduleView';
import SearchView from './components/SearchView';
import TopAiringView from './components/TopAiringView';
import AnimeModal from './components/AnimeModal';
import styles from './App.module.css';

export default function App() {
  const [activeTab, setActiveTab] = useState('season');
  const [searchQuery, setSearchQuery] = useState('');
  const [selectedAnime, setSelectedAnime] = useState(null);

  const handleSearch = (q) => {
    setSearchQuery(q);
    setActiveTab('search');
  };

  return (
    <div className={styles.app}>
      <Header
        onSearch={handleSearch}
        activeTab={activeTab}
        onTabChange={setActiveTab}
      />

      <main className={styles.main}>
        {activeTab === 'season' && (
          <SeasonView onCardClick={setSelectedAnime} />
        )}
        {activeTab === 'schedule' && (
          <ScheduleView onCardClick={setSelectedAnime} />
        )}
        {activeTab === 'top' && (
          <TopAiringView onCardClick={setSelectedAnime} />
        )}
        {activeTab === 'search' && searchQuery && (
          <SearchView query={searchQuery} onCardClick={setSelectedAnime} />
        )}
      </main>

      <footer className={styles.footer}>
        <p>
          Dati forniti da{' '}
          <a href="https://jikan.moe" target="_blank" rel="noopener noreferrer">
            Jikan API
          </a>{' '}
          · MyAnimeList
        </p>
      </footer>

      {selectedAnime && (
        <AnimeModal anime={selectedAnime} onClose={() => setSelectedAnime(null)} />
      )}
    </div>
  );
}
