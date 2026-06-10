import { useEffect, useState } from 'react';
import { getAnimeById } from '../api/jikan';
import styles from './AnimeModal.module.css';

export default function AnimeModal({ anime, onClose }) {
  const [full, setFull] = useState(null);
  const [loading, setLoading] = useState(true);

  useEffect(() => {
    if (!anime) return;
    document.body.style.overflow = 'hidden';
    setLoading(true);
    getAnimeById(anime.mal_id)
      .then((d) => setFull(d.data))
      .catch(() => setFull(anime))
      .finally(() => setLoading(false));
    return () => {
      document.body.style.overflow = '';
    };
  }, [anime]);

  useEffect(() => {
    const handler = (e) => e.key === 'Escape' && onClose();
    window.addEventListener('keydown', handler);
    return () => window.removeEventListener('keydown', handler);
  }, [onClose]);

  if (!anime) return null;

  const data = full || anime;
  const score = data.score?.toFixed(1);
  const trailer = data.trailer?.embed_url;

  return (
    <div className={styles.overlay} onClick={onClose}>
      <div className={styles.modal} onClick={(e) => e.stopPropagation()}>
        <button className={styles.close} onClick={onClose}>✕</button>

        {loading ? (
          <div className={styles.loader}>
            <div className={styles.spinner} />
          </div>
        ) : (
          <div className={styles.content}>
            <div className={styles.hero}>
              <img
                src={data.images?.jpg?.large_image_url || data.images?.jpg?.image_url}
                alt={data.title}
                className={styles.poster}
              />
              <div className={styles.heroInfo}>
                <h2 className={styles.title}>{data.title}</h2>
                {data.title_english && data.title_english !== data.title && (
                  <p className={styles.titleEn}>{data.title_english}</p>
                )}
                {data.title_japanese && (
                  <p className={styles.titleJp}>{data.title_japanese}</p>
                )}

                <div className={styles.badges}>
                  {data.type && <span className={styles.badge}>{data.type}</span>}
                  {data.status && (
                    <span
                      className={`${styles.badge} ${
                        data.status === 'Currently Airing' ? styles.badgeGreen : ''
                      }`}
                    >
                      {data.status === 'Currently Airing' ? '● In Onda' : data.status}
                    </span>
                  )}
                  {data.rating && (
                    <span className={styles.badge}>{data.rating.split(' ')[0]}</span>
                  )}
                </div>

                <div className={styles.stats}>
                  {score && (
                    <div className={styles.stat}>
                      <span className={styles.statLabel}>★ Punteggio</span>
                      <span className={styles.statValue}>{score}</span>
                    </div>
                  )}
                  {data.rank && (
                    <div className={styles.stat}>
                      <span className={styles.statLabel}># Rank</span>
                      <span className={styles.statValue}>#{data.rank}</span>
                    </div>
                  )}
                  {data.popularity && (
                    <div className={styles.stat}>
                      <span className={styles.statLabel}>Popolarità</span>
                      <span className={styles.statValue}>#{data.popularity}</span>
                    </div>
                  )}
                  {data.members && (
                    <div className={styles.stat}>
                      <span className={styles.statLabel}>Membri</span>
                      <span className={styles.statValue}>
                        {data.members.toLocaleString('it-IT')}
                      </span>
                    </div>
                  )}
                </div>

                <div className={styles.details}>
                  {data.episodes && (
                    <div className={styles.detail}>
                      <span>Episodi</span>
                      <span>{data.episodes}</span>
                    </div>
                  )}
                  {data.duration && (
                    <div className={styles.detail}>
                      <span>Durata</span>
                      <span>{data.duration}</span>
                    </div>
                  )}
                  {data.aired?.string && (
                    <div className={styles.detail}>
                      <span>Messa in onda</span>
                      <span>{data.aired.string}</span>
                    </div>
                  )}
                  {data.broadcast?.string && (
                    <div className={styles.detail}>
                      <span>Orario</span>
                      <span>{data.broadcast.string}</span>
                    </div>
                  )}
                  {data.studios?.length > 0 && (
                    <div className={styles.detail}>
                      <span>Studio</span>
                      <span>{data.studios.map((s) => s.name).join(', ')}</span>
                    </div>
                  )}
                  {data.source && (
                    <div className={styles.detail}>
                      <span>Fonte</span>
                      <span>{data.source}</span>
                    </div>
                  )}
                </div>

                {data.genres?.length > 0 && (
                  <div className={styles.genreList}>
                    {data.genres.map((g) => (
                      <span key={g.mal_id} className={styles.genre}>
                        {g.name}
                      </span>
                    ))}
                  </div>
                )}

                <a
                  href={data.url}
                  target="_blank"
                  rel="noopener noreferrer"
                  className={styles.malLink}
                >
                  Vedi su MyAnimeList →
                </a>
              </div>
            </div>

            {data.synopsis && (
              <div className={styles.synopsis}>
                <h3>Sinossi</h3>
                <p>{data.synopsis}</p>
              </div>
            )}

            {trailer && (
              <div className={styles.trailer}>
                <h3>Trailer</h3>
                <iframe
                  src={trailer}
                  title="Trailer"
                  allow="autoplay; encrypted-media"
                  allowFullScreen
                />
              </div>
            )}
          </div>
        )}
      </div>
    </div>
  );
}
